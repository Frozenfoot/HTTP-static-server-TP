//
// Created by frozenfoot on 12.10.17.
//

#ifndef UNTITLED_THREADPOOL_H
#define UNTITLED_THREADPOOL_H

#include <sys/types.h>
#include <queue>
#include <pthread.h>
#include "IThreadTask.h"

#define DEFAULT_POOL_SIZE 4
#define MAX_NCPU 0
#define DEFAULT_AFFINITY -1


class ThreadPool {
public:
    ThreadPool(size_t size, int ncpu);
    ~ThreadPool();
    void pushTask(IThreadTask **task);

private:
    class PooledThread;

    size_t poolSize;
    int usedCPU;
    bool stop = false;
    std::vector<ThreadPool::PooledThread*> threads;
    pthread_cond_t threadCondition = PTHREAD_COND_INITIALIZER;
    struct {
        std::queue<IThreadTask*> queue;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    } taskQueue;

    void cancelThreads();

    friend ThreadPool::PooledThread;

    class PooledThread{
    public:
        PooledThread(int id, ThreadPool *owner);
        PooledThread(int id, int cpu, ThreadPool *owner);

        int getID();

    private:
        int id;
        pthread_t pthread;
        ThreadPool *owner;

        static void *pthreadWrap (void *object);

        void threadLoop();
    };
};


#endif //UNTITLED_THREADPOOL_H
