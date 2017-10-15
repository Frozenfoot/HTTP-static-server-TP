//
// Created by frozenfoot on 15.10.17.
//

#ifndef UNTITLED_HANDLERTASK_H
#define UNTITLED_HANDLERTASK_H

#include <string>
#include "IThreadTask.h"
#include "Handler.h"

class HandlerTask : IThreadTask{
public:
    HandlerTask(Handler **client, std::string &root);
    ~HandlerTask();
    void execute();

private:
    Handler *client;
    std::string root;
};


#endif //UNTITLED_HANDLERTASK_H
