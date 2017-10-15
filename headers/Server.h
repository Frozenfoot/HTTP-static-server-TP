//
// Created by frozenfoot on 15.10.17.
//

#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include "../headers/ThreadPool.h"
#include <string>

#define DEFAULT_BACKLOG_SIZE 10

class Server {
public:
    Server(int port, const std::string &root, int threadPoolSize, int ncpu);
    ~Server();

    int start();
    void stop();

private:
    int port;
    int socket;
    std::string root;
    bool isWorking = false;
    ThreadPool *threadPool;

    void cleanUp();
};


#endif //UNTITLED_SERVER_H
