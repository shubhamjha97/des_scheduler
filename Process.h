#ifndef SCHEDULER_PROCESS_H
#define SCHEDULER_PROCESS_H

class Process {
public:
    int arrivalTime, totalCpuTime, cpuBurst, ioBurst;
    int cpuTimeRemaining, previousRemainingCpuBurst;
    int pid;
    int staticPriority, dynamicPriority;
    int cpuWaitTime, finishTimestamp, ioTime;
    int lastStateTimestamp;
    int orderIdx; // Used to keep the priority queues stable

    Process(int pid, int arrivalTime, int totalCpuTime, int cpuBurst, int ioBurst, int staticPriority, int dynamicPriority, int orderIdx);
};

#endif //SCHEDULER_PROCESS_H
