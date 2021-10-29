#include "Event.h"

Event::Event(Process *process, int timeStamp, ProcessState processState, Transition transition, int currentTimestamp) {
    this -> process = process;
    this -> processCurrState = processState;
    this -> timeStamp = timeStamp;
    this -> transition = transition;
    this -> eventGenerationTimestamp = currentTimestamp;
}

bool Event::Comparator::operator()(Event *eventA, Event *eventB) {
    if(eventA -> timeStamp == eventB -> timeStamp) {
        if(eventA->eventGenerationTimestamp == eventB-> eventGenerationTimestamp) {
            return eventA -> process -> pid > eventB -> process -> pid; // Especially useful when reading the input file
        }
        return eventA -> eventGenerationTimestamp > eventB -> eventGenerationTimestamp;
    }
    return eventA -> timeStamp > eventB -> timeStamp;
}
