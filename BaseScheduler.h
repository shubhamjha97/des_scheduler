#ifndef SCHEDULER_BASESCHEDULER_H
#define SCHEDULER_BASESCHEDULER_H

#include "Process.h"

class BaseScheduler {
    bool testPreempt; // TODO: Figure out if this is needed
    virtual void addProcess(Process* process) = 0;
    virtual Process* getNextProcess() = 0;
};

#endif //SCHEDULER_BASESCHEDULER_H
