#include "Process.h"

Process::Process(int pid, int arrivalTime, int totalCpuTime, int cpuBurst, int ioBurst) {
    this -> pid = pid;
    this -> arrivalTime = arrivalTime;
    this -> totalCpuTime = totalCpuTime;
    this -> cpuBurst = cpuBurst;
    this -> ioBurst = ioBurst;
}
