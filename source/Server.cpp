//
// Created by frozenfoot on 15.10.17.
//

#include "../headers/Server.h"
#include "../headers/http.h"
#include "../headers/Handler.h"
#include "../headers/HandlerTask.h"


#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

#define CHUNK 256

Server::Server(int port, const std::string &root, int threadPoolSize, int ncpu) {
    this->port = port;
    socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket <= 0){
        throw std::runtime_error("Unable to create socket");
    }

    int options = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options));

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    int binded = bind(socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    if(binded < 0){
        std::cout << "socket bind error" << std::endl;
        close(socket);
        throw std::runtime_error("Unable to bind socket");
    }

    this->root = root;
    threadPool = new ThreadPool(threadPoolSize, ncpu);

    listen(socket, DEFAULT_BACKLOG_SIZE);
}

Server::~Server(){
    cleanUp();
    if(socket > 0){
        close(socket);
    }
}

void Server::cleanUp() {
    if(threadPool != nullptr){
        delete threadPool;
    }
}

int Server::start() {
    if(socket <= 0){
        std::cout << "Server start error" << std::endl;
        throw new std::runtime_error("Socket was not handled");
    }

    if(isWorking){
        return 0;
    }

    isWorking = true;

    while(isWorking){
        struct sockaddr_in clientAddress;
        memset(&clientAddress, 0, sizeof(clientAddress));
        socklen_t clientLength = sizeof(clientAddress);

        int clientSocket = accept(socket, (struct sockaddr*)&clientAddress, &clientLength);
        Handler *client = new Handler(clientSocket);

        HandlerTask *task = new HandlerTask(&client, root);
        threadPool->pushTask((IThreadTask**) &task);
    }

    return 0;
}

void Server::stop() {
    if(socket <= 0){
        std::cout << "socket stop error" << std::endl;
        throw new std::runtime_error("Socket was not handled");
    }

    if(isWorking){
        isWorking = false;
    }
}