#include "RrScheduler.h"
#include "Process.h"

using namespace std;

void RrScheduler::addProcess(Process* process) {
    runQueue.push(process);
}

bool RrScheduler::testPreempt(Process* process, int currTime) {
    return false;
}

Process* RrScheduler::getNextProcess() {
    if(runQueue.empty()) {
        return nullptr;
    }
    Process* process = runQueue.front();
    runQueue.pop();
    return process;
}

string RrScheduler::getSchedulerName() {
    return "RR";
}

RrScheduler::RrScheduler(int quantum, int maxPrio) : BaseScheduler(quantum, maxPrio) {}
