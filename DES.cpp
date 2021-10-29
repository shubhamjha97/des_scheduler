#include "Event.h"
#include "BaseScheduler.h"
#include <queue>
#include <map>
#include "RNG.h" // TODO: Move RNG class to its header file

class DES {

private:
    template<typename T> class DesPriorityQueue : public std::priority_queue<T, std::vector<T>, Event::Comparator> {
    public:
        // TODO: make sure that the priority_queue is stable
        void remove(const T& value) {
            auto it = std::find(this->c.begin(), this->c.end(), value);
            if (it != this->c.end()) {
                this->c.erase(it);
                std::make_heap(this->c.begin(), this->c.end(), this->comp);
            }
        }
    };

    int CURRENT_TIME;
    DesPriorityQueue<Event*> DesQueue;
    BaseScheduler* scheduler;
    RNG* rng;
    Process* CURRENT_RUNNING_PROCESS;

public:
    map<Transition, string> DEBUG_TRANSITION_MAP{
        {TRANS_TO_READY, "READY"},
        {TRANS_TO_RUN, "RUNNG"},
        {TRANS_TO_BLOCK, "BLOCK"},
        {TRANS_TO_PREEMPT, "READY"}
    };
    map<ProcessState, string> DEBUG_STATE_MAP{
        {CREATED, "CREATED"},
        {READY, "READY"},
        {RUNNING, "RUNNG"},
        {BLOCKED, "BLOCK"}
    };

    DES(BaseScheduler* scheduler, RNG* rng) {
        this -> scheduler = scheduler;
        this -> rng = rng;
        CURRENT_TIME = 0;
        CURRENT_RUNNING_PROCESS = nullptr;
    }

    void runSimulation() {
        Event* event;
        int timeInPrevState;
        bool CALL_SCHEDULER = false;

        while( (event = getEvent()) ) {
            Process *proc = event -> process;
            CURRENT_TIME = event -> timeStamp;
            timeInPrevState = CURRENT_TIME - proc->lastStateTimestamp;

            printf("%d %d %d: %s -> %s\n", CURRENT_TIME, proc->pid, timeInPrevState, DEBUG_STATE_MAP[event->processCurrState].c_str(), DEBUG_TRANSITION_MAP[event->transition].c_str());
            switch(event -> transition) {
                case TRANS_TO_READY:
                    // must come from BLOCKED or from PREEMPTION
                    if(event -> processCurrState == BLOCKED) {
                        proc -> ioTime += timeInPrevState;
                        proc -> dynamicPriority = (proc -> staticPriority - 1);
                    }
                    transitionToReady(proc);
                    CALL_SCHEDULER = true;
                    break;
                case TRANS_TO_RUN:
                    if(event -> processCurrState == READY) {
                        proc -> cpuWaitTime += timeInPrevState;
                    }
                    transitionToRunning(proc, true); // create event for either preemption or blocking
                    break;
                case TRANS_TO_BLOCK:
                    transitionToBlock(proc);
                    CURRENT_RUNNING_PROCESS = nullptr;
                    CALL_SCHEDULER = true;
                    break;
                case TRANS_TO_PREEMPT:
                    // TODO: add to runqueue (no event is generated)
                    // TODO: Make current_running_process null
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

    void transitionToReady(Process* process) {
        scheduler->addProcess(process); // add to run queue
    }

    void transitionToRunning(Process* process, bool processAlreadyRunning) {
        // Transition to RUNNING
        int cpuBurstTime = min(rng->random(process->cpuBurst), process->cpuTimeRemaining);
        process -> cpuTimeRemaining -= cpuBurstTime; // Reduce cpuTimeRemaining when the process runs
        if(process -> cpuTimeRemaining > 0) { // Put the process in BLOCKED state only if it's not finished yet
            putEvent(new Event(process, CURRENT_TIME + cpuBurstTime, RUNNING, TRANS_TO_BLOCK, CURRENT_TIME));
        } else {
            process -> finishTimestamp = CURRENT_TIME + cpuBurstTime;
            CURRENT_RUNNING_PROCESS = nullptr;
            cout<<process->finishTimestamp<<" "<<process->pid<<" Done"<<endl;
        }
    }

    void transitionToBlock(Process* process) {
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
