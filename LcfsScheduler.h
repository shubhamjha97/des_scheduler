#ifndef SCHEDULER_LCFSSCHEDULER_H
#define SCHEDULER_LCFSSCHEDULER_H

#include "Process.h"
#include "BaseScheduler.h"
#include <stack>

using namespace std;

class LcfsScheduler : public BaseScheduler {
    stack<Process*> runStack;
public:
    LcfsScheduler(int quantum, int maxPrio);

    void addProcess(Process* process) override;

    bool testPreempt(Process* process, int currTime) override;

    Process* getNextProcess() override;

    string getSchedulerName() override;
};

#endif //SCHEDULER_LCFSSCHEDULER_H
