#ifndef SCHEDULER_BASESCHEDULER_H
#define SCHEDULER_BASESCHEDULER_H

#include "Process.h"
#include <string>

using namespace std;

class BaseScheduler {
protected:
    int quantum, maxPrio;
public:
    virtual bool testPreempt(Process* process, int currTime) = 0;
    virtual void addProcess(Process* process) = 0;
    virtual Process* getNextProcess() = 0;
    virtual string getSchedulerName() = 0;
    BaseScheduler(int quantum, int maxPrio) {
        this -> quantum = quantum;
        this -> maxPrio = maxPrio;
    }
};

#endif //SCHEDULER_BASESCHEDULER_H
