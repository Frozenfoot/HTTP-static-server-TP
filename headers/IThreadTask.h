//
// Created by frozenfoot on 12.10.17.
//

#ifndef UNTITLED_ITHREADTASK_H
#define UNTITLED_ITHREADTASK_H

class IThreadTask {
public:
    virtual ~IThreadTask() {};
    virtual void execute() = 0;
};

#endif //UNTITLED_ITHREADTASK_H
