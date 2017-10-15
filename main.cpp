#include <iostream>
#include <thread>
#include <cstring>

#include "headers/Server.h"

#define SERVER_PORT 80

int main(int argc, char* argv[]) {
    std::string rootPath;
    int ncpu = 0;
    for(auto i = 0; i < argc; ++i){
        if(strcmp(argv[i], "-r") == 0){
            rootPath = argv[i];
        }
        else if(strcmp(argv[i], "-c") == 0){
            ncpu = atoi(argv[i]);
        }
    }

    if(rootPath.length() == 0){
        rootPath = "/var/www/html";
        std::cout << "Using default root" << std::endl;
    }

    Server server(SERVER_PORT, rootPath, DEFAULT_POOL_SIZE, 4);
    server.start();

    return 0;
}