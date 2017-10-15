//
// Created by frozenfoot on 12.10.17.
//

#include "../headers/ThreadPool.h"
#include <unistd.h>
#include <stdexcept>
#include <iostream>

ThreadPool::ThreadPool(size_t size, int ncpu) {
    int totalCPU = (int) sysconf(_SC_NPROCESSORS_ONLN);
    if((size < 0) || (ncpu < -1)){
        std::cout << "Invalid thread pool parameters" << std::endl;
        throw new std::runtime_error("Invalid thread pool parameters");
    }

    if(ncpu > totalCPU){
        std::cout << "Invalid number of CPU" << std::endl;
        throw new std::runtime_error("Invalid number of NCPU");
    }

    if(ncpu == MAX_NCPU){
        usedCPU = totalCPU;
    }
    else{
        usedCPU = ncpu;
    }

    if(size == DEFAULT_POOL_SIZE) {
        poolSize = totalCPU + 1;
    }
    else{
        poolSize = size;
    }

    int id = 0;
    int cpu = 0;

    for(auto i = 0; i < poolSize; ++i){
        if(usedCPU != DEFAULT_AFFINITY){
            threads.push_back(new PooledThread(id, cpu, this));
            ++id;
            ++cpu;
            if(cpu >= usedCPU){
                cpu = 0;
            }
        }
        else{
            threads.push_back(new PooledThread(id, this));
            ++id;
        }
    }
}

ThreadPool::~ThreadPool() {
    if(!stop){
        cancelThreads();
    }

    for(auto i = 0; i < poolSize; ++i){
        delete threads[i];
    }
}

void ThreadPool::pushTask(IThreadTask **task) {
    pthread_mutex_lock(&taskQueue.mutex);
    taskQueue.queue.push(*task);
    pthread_cond_signal(&threadCondition);
    pthread_mutex_unlock(&taskQueue.mutex);
}

void ThreadPool::cancelThreads() {
    pthread_mutex_lock(&taskQueue.mutex);
    stop = true;
    pthread_mutex_unlock(&taskQueue.mutex);
    pthread_cond_broadcast(&threadCondition);
}

void *ThreadPool::PooledThread::pthreadWrap(void *object) {
    reinterpret_cast<PooledThread*>(object)->threadLoop();
}

ThreadPool::PooledThread::PooledThread(int id, ThreadPool *owner) {
    this->id = id;
    this->owner = owner;

    auto error = pthread_create(&pthread, nullptr, PooledThread::pthreadWrap, this);
    if(error != 0){
        std::cout << "Could not create thread" << std::endl;
        throw new std::runtime_error("Error creating thread " + std::to_string(id));
    }

    error = pthread_detach(pthread);

    if(error != 0){
        std::cout << "Could not attach thread" << std::endl;
        throw new std::runtime_error("Error detaching thread " + std::to_string(id));
    }

}

ThreadPool::PooledThread::PooledThread(int id, int cpu, ThreadPool *owner) {
    this->id = id;
    this->owner = owner;

    auto error = pthread_create(&pthread, nullptr, PooledThread::pthreadWrap, this);
    if(error != 0){
        throw new std::runtime_error("Error creating thread " + std::to_string(id));
    }

    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);
    CPU_SET(cpu, &cpuSet);

    error = pthread_setaffinity_np(pthread, sizeof(cpu_set_t), &cpuSet);

    if(error != 0){
        throw new std::runtime_error("Error setting CPU affinity" + std::to_string(id));
    }

    error = pthread_detach(pthread);

    if(error != 0){
        throw new std::runtime_error("Error detaching thread " + std::to_string(id));
    }
}

int ThreadPool::PooledThread::getID() {
    return id;
}

void ThreadPool::PooledThread::threadLoop() {
    while(true){
        pthread_mutex_lock(&owner->taskQueue.mutex);
        while((!owner->stop) && owner->taskQueue.queue.empty()){
            pthread_cond_wait(&owner->threadCondition, &owner->taskQueue.mutex);
        }

        if(owner->stop){
            pthread_mutex_unlock(&owner->taskQueue.mutex);
            return;
        }

        IThreadTask *task = owner->taskQueue.queue.front();
        owner->taskQueue.queue.pop();
        pthread_mutex_unlock(&owner->taskQueue.mutex);

        task->execute();
        delete task;
    }
}
