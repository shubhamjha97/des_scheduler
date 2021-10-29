#ifndef SCHEDULER_PRIOSCHEDULER_H
#define SCHEDULER_PRIOSCHEDULER_H

#include "Process.h"
#include "BaseScheduler.h"
#include <queue>
#include <vector>

using namespace std;

class PrioScheduler : public BaseScheduler {
    // TODO: Implement as pointers?
    vector<queue<Process*>> runQueue;
    vector<queue<Process*>> expiredQueue;
public:
    PrioScheduler(int quantum, int maxPrio);

    void addProcess(Process* process) override;

    bool testPreempt(Process* process, int currTime) override;

    Process* getNextProcess() override;

    string getSchedulerName() override;

    bool runQueueEmpty();

    bool expiredQueueEmpty();
};

#endif //SCHEDULER_PRIOSCHEDULER_H
