#ifndef SCHEDULER_FCFSSCHEDULER_H
#define SCHEDULER_FCFSSCHEDULER_H

#include "Process.h"
#include "BaseScheduler.h"
#include <queue>

using namespace std;

class FcfsScheduler : public BaseScheduler {
    queue<Process*> runQueue;
public:
    FcfsScheduler(int quantum, int maxPrio);

    void addProcess(Process* process) override;

    bool testPreempt(Process* process, int currTime) override;

    Process* getNextProcess() override;

    string getSchedulerName() override;
};

#endif //SCHEDULER_FCFSSCHEDULER_H
