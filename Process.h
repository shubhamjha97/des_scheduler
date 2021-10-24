#ifndef SCHEDULER_PROCESS_H
#define SCHEDULER_PROCESS_H

class Process {
public:
    int arrivalTime, totalCpuTime, cpuBurst, ioBurst;
    int pid;

    Process(int pid, int arrivalTime, int totalCpuTime, int cpuBurst, int ioBurst);
};

#endif //SCHEDULER_PROCESS_H
