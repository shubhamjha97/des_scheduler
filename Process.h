#ifndef SCHEDULER_PROCESS_H
#define SCHEDULER_PROCESS_H

class Process {
public:
    int arrivalTime, totalCpuTime, cpuBurst, ioBurst;
    int pid;
    int staticPriority, dynamicPriority; // TODO: initialize priority values
    int cpuWaitTime, finishTime, turnaroundTime, ioTime;

    Process(int pid, int arrivalTime, int totalCpuTime, int cpuBurst, int ioBurst, int staticPriority, int dynamicPriority);
};

#endif //SCHEDULER_PROCESS_H
