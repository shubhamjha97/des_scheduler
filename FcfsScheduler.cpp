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

    Process* getNextProcess() override {
        Process* process = runQueue.front();
        runQueue.pop();
        return process;
    }

};
