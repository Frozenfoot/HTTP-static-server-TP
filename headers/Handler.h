//
// Created by frozenfoot on 15.10.17.
//

#ifndef UNTITLED_HANDLER_H
#define UNTITLED_HANDLER_H

#include <cstddef>
#include <string>

class Handler {
private:
    int socket;

public:
    Handler(int socket);
    ~Handler();
    long sendRawResponse(const std::string &data);
    long sendRawResponse(const char *data, ssize_t length);
    std::string recieveRawRequest();

};


#endif //UNTITLED_HANDLER_H
