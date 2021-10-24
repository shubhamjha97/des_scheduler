#include "BaseScheduler.h"
#include "Process.h"
#include <queue>

using namespace std;

class FcfsScheduler : public BaseScheduler {
    queue<Process*> runQueue;
public:
    void addProcess(Process* process) override {
        runQueue.push(process);
    }

    bool testPreempt(Process* process, int currTime) override {
        return false;
    }

    Process* getNextProcess() override {
        Process* process = runQueue.front();
        runQueue.pop();
        return process;
    }
};
