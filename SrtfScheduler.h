#ifndef SCHEDULER_SRTFSCHEDULER_H
#define SCHEDULER_SRTFSCHEDULER_H

#include "BaseScheduler.h"
#include<queue>

class SrtfScheduler : public BaseScheduler {
    class Comparator {
    public:
        bool operator() (Process* processA, Process* processB);
    };
    class SrtfPriorityQueue : public std::priority_queue<Process*, std::vector<Process*>, SrtfScheduler::Comparator> {};
    SrtfPriorityQueue runQueue;

public:
    SrtfScheduler(int quantum, int maxPrio);

    void addProcess(Process* process) override;

    bool testPreempt(Process* process, int currTime) override;

    Process* getNextProcess() override;

    string getSchedulerName() override;
};

#endif //SCHEDULER_SRTFSCHEDULER_H
