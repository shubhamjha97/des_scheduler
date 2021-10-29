#include "PrePrioScheduler.h"

bool PrePrioScheduler::testPreempt(Process* process, int currTime) {
    return true;
}

string PrePrioScheduler::getSchedulerName() {
    return "PREPRIO";
}