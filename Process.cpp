#include "Process.h"

Process::Process(
        int pid,
        int arrivalTime,
        int totalCpuTime,
        int cpuBurst,
        int ioBurst,
        int staticPriority,
        int dynamicPriority) {
    this -> pid = pid;
    this -> arrivalTime = arrivalTime;
    this -> totalCpuTime = totalCpuTime;
    this -> cpuBurst = cpuBurst;
    this -> ioBurst = ioBurst;
    this -> staticPriority = staticPriority;
    this -> dynamicPriority = dynamicPriority;
    this -> cpuWaitTime = 0;
    this -> finishTime = 0;
    this -> turnaroundTime = 0;
    this -> ioTime = 0;
}
