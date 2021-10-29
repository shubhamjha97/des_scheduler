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
    Process* CURRENT_RUNNING_PROCESS;

public:
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

            cout<<CURRENT_TIME<<" "<<event->process->pid<<" "<<event->processCurrState<<" "<<event->transition<<" "<<event->process->cpuTimeRemaining<<endl; // TODO: remove
            switch(event -> transition) {
                case TRANS_TO_READY:
                    // must come from BLOCKED or from PREEMPTION
                    if(event -> processCurrState == BLOCKED) {
                        proc -> ioTime += timeInPrevState;
                    }
                    transitionToReady(proc);
                    CALL_SCHEDULER = (CURRENT_RUNNING_PROCESS==nullptr); // TODO: conditional on whether something is run?
                    break;
                case TRANS_TO_RUN:
                    if(event -> processCurrState == READY) {
                        proc -> cpuWaitTime += timeInPrevState;
                    }
                    transitionToRunning(proc, true); // create event for either preemption or blocking
                    break;
                case TRANS_TO_BLOCK:
                    transitionToBlock(proc);
                    CALL_SCHEDULER = true;
                    break;
                case TRANS_TO_PREEMPT:
                    // TODO: add to runqueue (no event is generated)
                    CALL_SCHEDULER = true;
                    // TODO: Make current_running_process null
                    break;
            }
            proc->lastStateTimestamp = CURRENT_TIME;
            delete event; event = nullptr; // remove current event object from Memory

            if(CALL_SCHEDULER) {
                if (getNextEventTime() == CURRENT_TIME)
                    continue; //process next event from Event queue

                // This flag is true whenever we want to get a new process from the scheduler
                CALL_SCHEDULER = false; // reset global flag
//                CURRENT_RUNNING_PROCESS = nullptr; // TODO: Check
                if (CURRENT_RUNNING_PROCESS == nullptr) {
                    CURRENT_RUNNING_PROCESS = scheduler->getNextProcess();
                    if (CURRENT_RUNNING_PROCESS == nullptr)
                        continue;
                    putEvent(new Event(CURRENT_RUNNING_PROCESS, CURRENT_TIME, READY, TRANS_TO_RUN, CURRENT_TIME));

                    // TODO: create event to make this process runnable for same time.
                }

            }
        }
    }

    void transitionToReady(Process* process) {
        scheduler->addProcess(process); // add to run queue
    }

    void transitionToRunning(Process* process, bool processAlreadyRunning) {
        // Transition to BLOCKED
        // TODO: make sure random number is generated in the correct range
        int cpuBurstTime = min(rng->random(process->cpuBurst), process->cpuTimeRemaining);
        process -> cpuTimeRemaining -= cpuBurstTime; // Reduce cpuTimeRemaining when the process runs
        if(process -> cpuTimeRemaining > 0) { // Put the process in BLOCKED state only if it's not finished yet
            putEvent(new Event(process, CURRENT_TIME + cpuBurstTime, RUNNING, TRANS_TO_BLOCK, CURRENT_TIME));
        } else {
            process -> finishTimestamp = CURRENT_TIME + cpuBurstTime; // TODO: currentTime + cpuBurstTime
            CURRENT_RUNNING_PROCESS = nullptr;
        }
    }

    void transitionToBlock(Process* process) {
        if(process -> cpuTimeRemaining > 0) {
            int ioBurstTime = rng->random(process->ioBurst);
            // Transition to READY
            putEvent(new Event(process, CURRENT_TIME + ioBurstTime, BLOCKED, TRANS_TO_READY, CURRENT_TIME));
        }
        CURRENT_RUNNING_PROCESS = nullptr;
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
