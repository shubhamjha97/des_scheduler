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
    const char* delimiters = " \t\n";

    while(getline(infile, line)){   // read a line
        vector<int> processParams;
        stringstream ss(line);

        char* token = strtok(&line[0], delimiters);
        while (token != NULL) {
            processParams.push_back(stoi((string)token));
            token = strtok (NULL, delimiters);
        }

        if(processParams.size() != 4) {
            break;
        }

        int staticPriority = rng -> random(MAX_PRIO);
        int dynamicPriority = staticPriority - 1; // TODO: Check if this is correct

        processes.push_back(
                new Process(
                        currentPid,
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

    int numProcess = processes.size();
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
    int lastEventFinishTimestamp = INT_MIN, timeCpuBusy = 0, timeIoBusy = 0;
    double cpuUtilization, ioUtilization;
    double  avgTurnaroundTime=0.0, avgCpuWaitingTime=0.0, throughput=0.0;

    for(auto process: processes) {
        lastEventFinishTimestamp = max(lastEventFinishTimestamp, process -> finishTimestamp);
        avgCpuWaitingTime += process->cpuWaitTime;
        avgTurnaroundTime += (process->finishTimestamp - process->arrivalTime);
        timeCpuBusy += process->totalCpuTime;
        timeIoBusy += process->ioTime; // TODO: Fix calculation
    }
    avgCpuWaitingTime /= numProcess;
    avgTurnaroundTime /= numProcess;

    throughput = 100.0 * numProcess / (double) lastEventFinishTimestamp;

    cpuUtilization = 100.0 * (timeCpuBusy / (double) lastEventFinishTimestamp);
    ioUtilization = 100.0 * (timeIoBusy / (double) lastEventFinishTimestamp);

    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
           lastEventFinishTimestamp,
           cpuUtilization,
           ioUtilization,
           avgTurnaroundTime,
           avgCpuWaitingTime,
           throughput);

}
#endif //SCHEDULER_HELPER_H
