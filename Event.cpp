
enum Transition {TRANS_TO_READY, TRANS_TO_RUN, TRANS_TO_BLOCK, TRANS_TO_PREEMPT};
enum ProcessState {CREATED, READY, RUNNING, BLOCKED};

class Event {
public:
    Process *process;
    int timeStamp;
    ProcessState processOldState;
    Transition transition;

    Event(Process *process, int timeStamp, ProcessState processState, Transition transition) {
        this -> process = process;
        this -> processOldState = processState;
        this -> timeStamp = timeStamp;
        this-> transition = transition;
    }

    class Comparator {
    public:
        bool operator() (Event* eventA, Event* eventB) {
            // TODO: comparison code here
            return true;
        }
    };
};
