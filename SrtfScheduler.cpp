#include "SrtfScheduler.h"
#include "Process.h"

using namespace std;

void SrtfScheduler::addProcess(Process* process) {
    runQueue.push(process);
}

bool SrtfScheduler::testPreempt(Process* process, int currTime) {
    return false;
}

Process* SrtfScheduler::getNextProcess() {
    if(runQueue.empty()) {
        return nullptr;
    }
    Process* process = runQueue.top();
    runQueue.pop();
    return process;
}

string SrtfScheduler::getSchedulerName() {
    return "SRTF";
}

SrtfScheduler::SrtfScheduler(int quantum, int maxPrio) : BaseScheduler(quantum, maxPrio) {}

bool SrtfScheduler::Comparator::operator()(Process *processA, Process *processB) {
    if(processA->cpuTimeRemaining == processB->cpuTimeRemaining) {
        if(processA->lastStateTimestamp == processB->lastStateTimestamp) {
            return processA -> pid > processB -> pid;
        }
        return processA->lastStateTimestamp > processB->lastStateTimestamp;
    }
    return processA->cpuTimeRemaining > processB->cpuTimeRemaining;
}
