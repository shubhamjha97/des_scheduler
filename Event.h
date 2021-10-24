#ifndef SCHEDULER_EVENT_H
#define SCHEDULER_EVENT_H

enum Transition {TRANS_TO_READY, TRANS_TO_RUN, TRANS_TO_BLOCK, TRANS_TO_PREEMPT};
enum ProcessState {CREATED, READY, RUNNING, BLOCKED};

class Event {
public:
    Event(Process *process, int timeStamp, ProcessState processState, Transition transition);

    Process *process;
    int timeStamp;
    ProcessState processOldState;
    Transition transition;

    class Comparator {
    public:
        bool operator() (Event* eventA, Event* eventB);
    };
};

#endif //SCHEDULER_EVENT_H
