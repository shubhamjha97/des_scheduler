#ifndef SCHEDULER_PROCESS_H
#define SCHEDULER_PROCESS_H

class Process {
public:
    int arrivalTime, totalCpuTime, cpuBurst, ioBurst;
    int cpuTimeRemaining;
    int pid;
    int staticPriority, dynamicPriority; // TODO: initialize priority values
    int cpuWaitTime, finishTimestamp, ioTime;
    int lastStateTimestamp;

    Process(int pid, int arrivalTime, int totalCpuTime, int cpuBurst, int ioBurst, int staticPriority, int dynamicPriority);
};

#endif //SCHEDULER_PROCESS_H
