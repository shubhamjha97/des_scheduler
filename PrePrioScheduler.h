//
// Created by Shubham Jha on 10/29/21.
//

#ifndef SCHEDULER_PREPRIOSCHEDULER_H
#define SCHEDULER_PREPRIOSCHEDULER_H

#include "Process.h"
#include "PrioScheduler.h"

class PrePrioScheduler : public PrioScheduler {
public:
    PrePrioScheduler(int quantum, int maxPrio) : PrioScheduler(quantum, maxPrio){}
    bool testPreempt(Process* process, int currTime) override;
    string getSchedulerName() override;
};

#endif //SCHEDULER_PREPRIOSCHEDULER_H
