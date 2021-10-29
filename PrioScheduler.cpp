#include "PrioScheduler.h"

using namespace std;

void PrioScheduler::addProcess(Process* process) {
    if(process -> dynamicPriority == -1) {
        process -> dynamicPriority = process -> staticPriority - 1;
        expiredQueue[process->dynamicPriority].push(process);
    } else {
        runQueue[process->dynamicPriority].push(process);
    }
}

bool PrioScheduler::testPreempt(Process* process, int currTime) {
    return false;
}

bool PrioScheduler::runQueueEmpty() {
    bool result = true;
    for(int i=0; i<maxPrio; i++) {
        result &= runQueue[i].empty();
    }
    return result;
}

bool PrioScheduler::expiredQueueEmpty() {
    bool result = true;
    for(int i=0; i<maxPrio; i++) {
        result &= expiredQueue[i].empty();
    }
    return result;
}

Process* PrioScheduler::getNextProcess() {
    if(runQueueEmpty()) {
        if(expiredQueueEmpty()) {
            return nullptr;
        }
        swap(runQueue, expiredQueue);
    }

    int highestPopulatedLevel = maxPrio - 1;
    while(runQueue[highestPopulatedLevel].empty()) {
        highestPopulatedLevel--;
    }

    Process* process = runQueue[highestPopulatedLevel].front();
    runQueue[highestPopulatedLevel].pop();
    return process;
}

string PrioScheduler::getSchedulerName() {
    return "PRIO";
}

PrioScheduler::PrioScheduler(int quantum, int maxPrio) : BaseScheduler(quantum, maxPrio) {
    this -> runQueue = vector<queue<Process*>>(maxPrio, queue<Process*>());
    this -> expiredQueue = vector<queue<Process*>>(maxPrio, queue<Process*>());
}
