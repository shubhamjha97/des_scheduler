#include "Event.h"
#include "BaseScheduler.h"
#include <queue>
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

public:
    DES(BaseScheduler* scheduler, RNG* rng) {
        this -> scheduler = scheduler;
        this -> rng = rng;
        CURRENT_TIME = 0;
    }

    void runSimulation() {
        Event* event;
//        int timeInPrevState;
        bool CALL_SCHEDULER = false;
        Process* CURRENT_RUNNING_PROCESS = nullptr;

        while( (event = getEvent()) ) {
            Process *proc = event -> process;
            CURRENT_TIME = event -> timeStamp;
//            timeInPrevState = CURRENT_TIME – proc->state_ts; // TODO: Useful for accounting

            switch(event -> transition) {
                case TRANS_TO_READY:
                    // must come from BLOCKED or from PREEMPTION
                    scheduler->addProcess(proc); // add to run queue
                    CALL_SCHEDULER = true; // TODO: conditional on whether something is run?
                    break;
                case TRANS_TO_RUN:
                    makeProcessRunning(proc, true); // create event for either preemption or blocking
                    break;
                case TRANS_TO_BLOCK:
                    makeProcessBlocked(proc);
                    CALL_SCHEDULER = true;
                    break;
                case TRANS_TO_PREEMPT:
                    // TODO: add to runqueue (no event is generated)
                    CALL_SCHEDULER = true;
                    break;
            }
            delete event; event = nullptr; // remove current event object from Memory

            if(CALL_SCHEDULER) {
                if (getNextEventTime() == CURRENT_TIME)
                    continue; //process next event from Event queue

                CALL_SCHEDULER = false; // reset global flag

                if (CURRENT_RUNNING_PROCESS == nullptr) {
                    CURRENT_RUNNING_PROCESS = scheduler->getNextProcess(); // TODO: initialize scheduler
                    if (CURRENT_RUNNING_PROCESS == nullptr)
                        continue;
                    makeProcessRunning(CURRENT_RUNNING_PROCESS, false);
                    // TODO: create event to make this process runnable for same time.
                }

            }
        }
    }

    void makeProcessRunning(Process* process, bool processAlreadyRunning) {
        if(!processAlreadyRunning) {
            // Transition to RUNNING
            putEvent(new Event(process, CURRENT_TIME, READY, TRANS_TO_RUN));
        }

        // Transition to BLOCKED
        // TODO: make sure random number is generated in the correct range
        int cpuBurstTime = min(rng->random(process->cpuBurst), process->totalCpuTime);
        process -> totalCpuTime -= cpuBurstTime; // Reduce totalCpuTime when the process runs
        if(process -> totalCpuTime > 0) { // Put the process in BLOCKED state only if it's not finished yet
            putEvent(new Event(process, CURRENT_TIME + cpuBurstTime, RUNNING, TRANS_TO_BLOCK));
        }
    }

    void makeProcessBlocked(Process* process) {
        if(process -> totalCpuTime > 0) {
            int ioBurstTime = rng->random(process->ioBurst);
            // Transition to READY
            putEvent(new Event(process, CURRENT_TIME + ioBurstTime, RUNNING, TRANS_TO_READY));
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
        // TODO: maybe delete event object
    }
};
