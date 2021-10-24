#ifndef SCHEDULER_BASESCHEDULER_H
#define SCHEDULER_BASESCHEDULER_H

#include "Process.h"

class BaseScheduler {
public:
    virtual bool testPreempt(Process* process, int currTime) = 0;
    virtual void addProcess(Process* process) = 0;
    virtual Process* getNextProcess() = 0;
};

#endif //SCHEDULER_BASESCHEDULER_H
