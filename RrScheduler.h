#ifndef SCHEDULER_RRSCHEDULER_H
#define SCHEDULER_RRSCHEDULER_H

#include "Process.h"
#include "BaseScheduler.h"
#include <queue>

using namespace std;

class RrScheduler : public BaseScheduler {
    queue<Process*> runQueue;
public:
    RrScheduler(int quantum, int maxPrio);

    void addProcess(Process* process) override;

    bool testPreempt(Process* process, int currTime) override;

    Process* getNextProcess() override;

    string getSchedulerName() override;
};

#endif //SCHEDULER_RRSCHEDULER_H
