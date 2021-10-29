#include "FcfsScheduler.h"
#include "Process.h"

using namespace std;

void FcfsScheduler::addProcess(Process* process) {
    runQueue.push(process);
}

bool FcfsScheduler::testPreempt(Process* process, int currTime) {
    return false;
}

Process* FcfsScheduler::getNextProcess() {
    if(runQueue.empty()) {
        return nullptr;
    }
    Process* process = runQueue.front();
    runQueue.pop();
    return process;
}

string FcfsScheduler::getSchedulerName() {
    return "FCFS";
}

FcfsScheduler::FcfsScheduler(int quantum, int maxPrio) : BaseScheduler(quantum, maxPrio) {}
