#ifndef SCHEDULER_HELPER_H
#define SCHEDULER_HELPER_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Process.h"
#include "RNG.h"

using namespace std;

static vector<Process*> readProcessFile(string &processFilePath, int maxPriority, RNG *rng) {
    vector<Process*> processes;
    ifstream infile(processFilePath);
    string line;
    int currentPid = 0;
    int MAX_PRIO = maxPriority;

    while(getline(infile, line)){   // read a line
        vector<int> processParams;
        stringstream ss(line);

        while(getline(ss, line, ' ')) { // split on spaces
            processParams.push_back(stoi(line));
        }
        if(processParams.size() != 4) {
            break;
        }

        int staticPriority = rng -> random(MAX_PRIO); // TODO: ensure random numbers are generated in the correct range
        int dynamicPriority = staticPriority - 1; // TODO: Check if this is correct

        processes.push_back(
                new Process(
                        currentPid, // TODO: Check whether pids are being allocated and incremented correctly
                        processParams[0],
                        processParams[1],
                        processParams[2],
                        processParams[3],
                        staticPriority,
                        dynamicPriority
                )
        );
        currentPid++;
    }

    return processes;
}

void dumpResultsToConsole(string schedulerName, int timeQuantum, const vector<Process*>& processes) {
    // TODO: Add scheduler info
    // TODO: use correct number of decimal places
    for(auto process: processes) {
        printf(
                "%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
                process->pid,
                process->arrivalTime,
                process->totalCpuTime,
                process->cpuBurst,
                process->ioBurst,
                process->staticPriority,
                process->finishTimestamp,
                process->finishTimestamp - process->arrivalTime,
                process->ioTime,
                process->cpuWaitTime);
    }
    // TODO: add summary info
    int lastEventFinishTimestamp = INT_MIN;
    for(auto process: processes) {
        lastEventFinishTimestamp = max(lastEventFinishTimestamp, process -> finishTimestamp);
    }
    double cpuUtilization, ioUtilization;
    double  avgTurnaroundTime, avgCpuWaitingTime, throughput;

    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
           lastEventFinishTimestamp,
           cpuUtilization,
           ioUtilization,
           avgTurnaroundTime,
           avgCpuWaitingTime,
           throughput);

}
#endif //SCHEDULER_HELPER_H
