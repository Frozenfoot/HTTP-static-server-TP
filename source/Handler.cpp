//
// Created by frozenfoot on 15.10.17.
//

#include "../headers/Handler.h"
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>

#define REQUEST_BUFFER_LENGTH 1024

Handler::Handler(int socket) {
    if(socket <= 0){
        std::cout << "socket error" << std::endl;
        throw new std::runtime_error("Incorrect socket");
    }
    this->socket = socket;
}

Handler::~Handler() {
    if(socket > 0){
        close(socket);
    }
}

long Handler::sendRawResponse(const std::string &data) {
    const char *charData = data.c_str();
    ssize_t dataLength = data.length();
    ssize_t left = strlen(charData);
    ssize_t sent = 0;
    ssize_t sentTotal = 0;

    while(left > 0){
        sent = ::send(socket, charData + sent, dataLength - sent, 0);
        if(sent == -1){
            return sentTotal;
        }
        left -= sent;
        sentTotal += sent;
    }
    return sentTotal;
}

long Handler::sendRawResponse(const char *data, ssize_t length) {
    ssize_t left = strlen(data);
    ssize_t sent = 0;
    ssize_t sentTotal = 0;

    while(left > 0){
        sent = ::send(socket, data + sent, length - sent, 0);
        if(sent == -1){
            return sentTotal;
        }
        left -= sent;
        sentTotal += sent;
    }
    return sentTotal;
}

std::string Handler::recieveRawRequest() {
    std::string result;
    char buffer[REQUEST_BUFFER_LENGTH];
    ssize_t read = recv(socket, buffer, REQUEST_BUFFER_LENGTH, 0);
    if(read > 0){
        result.append(buffer, read);
    }
    return result;
}
