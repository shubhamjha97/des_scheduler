#include "Process.cpp"

class BaseScheduler {
    bool testPreempt; // TODO: Figure out if this is needed
    virtual void addProcess(Process process) = 0;
    virtual Process getNextProcess() = 0;
};
