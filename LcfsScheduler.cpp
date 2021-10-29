#include "LcfsScheduler.h"
#include "Process.h"

using namespace std;

void LcfsScheduler::addProcess(Process* process) {
    runStack.push(process);
}

bool LcfsScheduler::testPreempt(Process* process, int currTime) {
    return false;
}

Process* LcfsScheduler::getNextProcess() {
    if(runStack.empty()) {
        return nullptr;
    }
    Process* process = runStack.top();
    runStack.pop();
    return process;
}

string LcfsScheduler::getSchedulerName() {
    return "LCFS";
}

LcfsScheduler::LcfsScheduler(int quantum, int maxPrio) : BaseScheduler(quantum, maxPrio) {}
