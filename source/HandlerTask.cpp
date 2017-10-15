//
// Created by frozenfoot on 15.10.17.
//

#include "../headers/HandlerTask.h"
#include "../headers/http.h"


#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <climits>
#include <iostream>

#define CHUNK 256

HandlerTask::HandlerTask(Handler **client, std::string &root) {
    this->client = *client;
    this->root = root;
}

HandlerTask::~HandlerTask() {
    if(client != nullptr){
        delete client;
    }
}

void HandlerTask::execute() {
    std::string request = client->recieveRawRequest();
    if(request.length() == 0){
        return;
    }

    std::string method;
    std::string path;
    Http::parseRequest(request, method, path);

    if((method.length() == 0) || (path.length() == 0)){
        client->sendRawResponse(HTTP_RAW_BAD_REQUEST);
        return;
    }

    if((method != GET_METHOD) && (method != HEAD_METHOD)){
        client->sendRawResponse(HTTP_RAW_NOT_ALLOWED);
        return;
    }

    bool needIndex = false;
    if(path[path.length() - 1] == '/') {
        path.append("index.html");
        needIndex = true;
    }

    std::string fullPath = root;
    fullPath.append(path);

    char absolutePath[PATH_MAX];
    if(realpath(fullPath.c_str(), absolutePath) == nullptr){
        if(needIndex){
            client->sendRawResponse(HTTP_RAW_FORBIDDEN);
            return;
        }
        client->sendRawResponse(HTTP_RAW_NOT_FOUND);
        return;
    }

    std::string filteredString = absolutePath;
    if(filteredString.find(root) == std::string::npos){
        client->sendRawResponse(HTTP_RAW_NOT_FOUND);
        return;
    }

    int file = open(absolutePath, O_RDONLY);
    if(file <= -1){
        client->sendRawResponse(HTTP_RAW_NOT_FOUND);
        return;
    }

    struct stat statBuffer;
    if(fstat(file, &statBuffer) != 0){
        client->sendRawResponse(HTTP_RAW_ERROR);
        return;
    }

    if(S_ISDIR(statBuffer.st_mode)){
        client->sendRawResponse(HTTP_RAW_NOT_FOUND);
        return;
    }

    ssize_t fileSize = statBuffer.st_size;
//    std::cout << "File " << absolutePath << " Size: " << fileSize << std::endl;
    std::string mimeType = Http::getMimeType(fullPath);
    std::string date = Http::getDate();
    client->sendRawResponse(Http::makeResponseHead(STATUS_OK, date, mimeType, fileSize, "Closed").c_str());

    if(method != HEAD_METHOD){
        char fileBuffer[CHUNK];
        long readBytes;
        long sentBytes;

        while((readBytes = read(file, fileBuffer, CHUNK)) > 0){
//            std::cout << "File " << absolutePath << " Read bytes " << readBytes << std::endl;
            sentBytes = client->sendRawResponse(fileBuffer, readBytes);
//            if(sentBytes < readBytes){
//                std::cout << "Error sending file " << fullPath << std::endl;
//                break;
//            }
        }
    }

    close (file);
}