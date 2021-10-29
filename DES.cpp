#include "Event.h"
#include "BaseScheduler.h"
#include <queue>
#include <map>
#include "RNG.h"

class DES {

private:
    template<typename T> class DesPriorityQueue : public std::priority_queue<T, std::vector<T>, Event::Comparator> {
    public:
        void remove(const T& value) {
            auto it = std::find(this->c.begin(), this->c.end(), value);
            if (it != this->c.end()) {
                this->c.erase(it);
                std::make_heap(this->c.begin(), this->c.end(), this->comp);
            }
        }
    };

    int CURRENT_TIME, quantum;
    bool vflag;
    map<int, Event*> blockPreemptExitMap;
    DesPriorityQueue<Event*> DesQueue;
    BaseScheduler* scheduler;
    RNG* rng;
    Process* CURRENT_RUNNING_PROCESS;

public:
    map<Transition, string> DEBUG_TRANSITION_MAP{
        {TRANS_TO_READY, "READY"},
        {TRANS_TO_RUN, "RUNNG"},
        {TRANS_TO_BLOCK, "BLOCK"},
        {TRANS_TO_PREEMPT, "PRE_READY"},
        {TRANS_TO_EXIT, "EXIT"},
    };
    map<ProcessState, string> DEBUG_STATE_MAP{
        {CREATED, "CREATED"},
        {READY, "READY"},
        {RUNNING, "RUNNG"},
        {BLOCKED, "BLOCK"},
    };

    DES(BaseScheduler* scheduler, RNG* rng, int quantum, bool vflag) {
        this -> scheduler = scheduler;
        this -> rng = rng;
        this -> quantum = quantum;
        this -> vflag = vflag;
        CURRENT_TIME = 0;
        CURRENT_RUNNING_PROCESS = nullptr;
    }

    void runSimulation(vector<pair<int, int>> &ioTimes) {
        Event* event;
        int timeInPrevState;
        bool CALL_SCHEDULER = false;

        while( (event = getEvent()) ) {
            Process *proc = event -> process;
            CURRENT_TIME = event -> timeStamp;
            timeInPrevState = CURRENT_TIME - proc->lastStateTimestamp;

            if(vflag && event -> transition != TRANS_TO_PREEMPT) {
                printf("%d %d %d: %s -> %s\n", CURRENT_TIME, proc->pid, timeInPrevState,
                       DEBUG_STATE_MAP[event->processCurrState].c_str(),
                       DEBUG_TRANSITION_MAP[event->transition].c_str());
            }

            switch(event -> transition) {
                case TRANS_TO_READY:
                    // must come from BLOCKED or from PREEMPTION
                    if(event -> processCurrState == BLOCKED) {
                        proc -> ioTime += timeInPrevState;
                        proc -> dynamicPriority = (proc -> staticPriority - 1);
                        ioTimes.push_back(make_pair(proc->lastStateTimestamp, CURRENT_TIME));
                    }
                    transitionToReady(proc);
                    CALL_SCHEDULER = true;
                    break;
                case TRANS_TO_RUN:
                    proc -> cpuWaitTime += timeInPrevState;
                    transitionToRunning(proc); // create event for either preemption or blocking
                    break;
                case TRANS_TO_BLOCK:
                    transitionToBlock(proc);
                    CURRENT_RUNNING_PROCESS = nullptr;
                    CALL_SCHEDULER = true;
                    break;
                case TRANS_TO_PREEMPT:
                    transitionToPreempt(proc);
                    CURRENT_RUNNING_PROCESS = nullptr;
                    CALL_SCHEDULER = true;
                    break;
                case TRANS_TO_EXIT:
                    transitionToExit(proc);
                    CALL_SCHEDULER = true;
                    break;
            }
            proc->lastStateTimestamp = CURRENT_TIME;
            delete event; event = nullptr; // remove current event object from Memory

            if(CALL_SCHEDULER) { // This flag is true whenever we want to get a new process from the scheduler
                if (getNextEventTime() == CURRENT_TIME) {
                    continue; // process all events at the same time before scheduling
                }
                CALL_SCHEDULER = false; // reset global flag
                if (CURRENT_RUNNING_PROCESS == nullptr) {
                    CURRENT_RUNNING_PROCESS = scheduler->getNextProcess();
                    if (CURRENT_RUNNING_PROCESS == nullptr)
                        continue;
                    // create event to make process runnable for same time.
                    putEvent(new Event(CURRENT_RUNNING_PROCESS, CURRENT_TIME, READY, TRANS_TO_RUN, CURRENT_TIME));
                }
            }
        }
    }

    void transitionToPreempt(Process* process) {
        process -> dynamicPriority--;

        int processRunTime = (CURRENT_TIME - process->lastStateTimestamp);
        process -> cpuTimeRemaining -= processRunTime;
        process -> previousRemainingCpuBurst -= processRunTime;

        putEvent(new Event(process, CURRENT_TIME, RUNNING, TRANS_TO_READY, CURRENT_TIME));
    }

    void transitionToReady(Process* process) {
        scheduler->addProcess(process); // add to run queue
    }

    void transitionToRunning(Process* process) {
        int cpuBurstTime;
        if(process -> previousRemainingCpuBurst > 0) {
            cpuBurstTime = process -> previousRemainingCpuBurst;
        } else {
            cpuBurstTime = rng->random(process->cpuBurst);
            process -> previousRemainingCpuBurst = cpuBurstTime;
        }
        process -> previousRemainingCpuBurst = min(process->previousRemainingCpuBurst, process->cpuTimeRemaining);

        if(process->previousRemainingCpuBurst > quantum) {
            Event *event = new Event(process, CURRENT_TIME + quantum, RUNNING, TRANS_TO_PREEMPT, CURRENT_TIME);
            putEvent(event);
            blockPreemptExitMap[process->pid] = event;
            return;
        }

        if((process->cpuTimeRemaining - process->previousRemainingCpuBurst) > 0) { // Put the process in BLOCKED state only if it's not finished yet
            Event *event = new Event(process, CURRENT_TIME + process->previousRemainingCpuBurst, RUNNING, TRANS_TO_BLOCK, CURRENT_TIME);
            putEvent(event);
            blockPreemptExitMap[process->pid] = event;
        } else {
            Event *event = new Event(process, CURRENT_TIME + process->previousRemainingCpuBurst, RUNNING, TRANS_TO_EXIT, CURRENT_TIME);
            putEvent(event);
            blockPreemptExitMap[process->pid] = event;
        }
    }

    void transitionToExit(Process* process) {
        process -> finishTimestamp = CURRENT_TIME;
        CURRENT_RUNNING_PROCESS = nullptr;
        if(vflag) {
            cout << process->finishTimestamp << " " << process->pid << " Done" << endl;
        }
    }

    void transitionToBlock(Process* process) {
        int processRunTime = (CURRENT_TIME - process->lastStateTimestamp);
        process -> cpuTimeRemaining -= processRunTime;
        process -> previousRemainingCpuBurst -= processRunTime;

        if(process -> cpuTimeRemaining > 0) {
            int ioBurstTime = rng->random(process->ioBurst);
            // Transition to READY
            putEvent(new Event(process, CURRENT_TIME + ioBurstTime, BLOCKED, TRANS_TO_READY, CURRENT_TIME));
        }
    }

    Event* getEvent() {
        if(DesQueue.empty()) {
            return nullptr;
        }

        Event *event = DesQueue.top();
        DesQueue.pop();
        return event;
    }

    int getNextEventTime() {
        if(DesQueue.empty()) {
            return -1;
        }
        return DesQueue.top()->timeStamp;
    }

    void putEvent(Event *event) {
        DesQueue.push(event);
    }

    void removeEvent(Event *event) {
        DesQueue.remove(event);
        delete event;
    }
};
