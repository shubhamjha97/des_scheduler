#ifndef SCHEDULER_EVENT_H
#define SCHEDULER_EVENT_H

#include "Process.h"

enum Transition {TRANS_TO_READY, TRANS_TO_RUN, TRANS_TO_BLOCK, TRANS_TO_PREEMPT, TRANS_TO_EXIT};
enum ProcessState {CREATED, READY, RUNNING, BLOCKED};

class Event {
public:
    Event(Process *process, int timeStamp, ProcessState processState, Transition transition, int currentTimestamp);

    Process *process;
    int timeStamp, eventGenerationTimestamp;
    ProcessState processCurrState;
    Transition transition;

    class Comparator {
    public:
        bool operator() (Event* eventA, Event* eventB);
    };
};

#endif //SCHEDULER_EVENT_H
