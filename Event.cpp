#include "Event.h"

Event::Event(Process *process, int timeStamp, ProcessState processState, Transition transition) {
    this -> process = process;
    this -> processOldState = processState;
    this -> timeStamp = timeStamp;
    this-> transition = transition;
}

bool Event::Comparator::operator()(Event *eventA, Event *eventB) {
    // TODO: comparison code here. Ensure that sorting is stable.
    return eventA -> timeStamp > eventB -> timeStamp;
}
