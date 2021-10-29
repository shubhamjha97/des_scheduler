#ifndef SCHEDULER_HELPER_H
#define SCHEDULER_HELPER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Process.h"
#include "RNG.h"
#include "BaseScheduler.h"
#include "FcfsScheduler.h"
#include "LcfsScheduler.h"
#include "SrtfScheduler.h"
#include "RrScheduler.h"
#include "PreScheduler.h"
#include "PrePrioScheduler.h"

using namespace std;

enum Schedulers {FCFS, LCFS, SRTF, RR, PRIO, PREPRIO}; // TODO: Maybe remove

int DEFAULT_QUANTUM = 10000;
int DEFAULT_MAXPRIO = 4;

int parseForQuantum(string schedSpec) {
    int n = schedSpec.size();
    string buffer;
    for(int i=1; i<n; i++) {
        if(schedSpec[i] == ':') {
            break;
        }
        buffer.push_back(schedSpec[i]);
    }
    return buffer.empty() ? DEFAULT_QUANTUM : stoi(buffer);
}

int parseForMaxPrio(string schedSpec) {
    int n = schedSpec.size();
    string buffer;
    int i=1;
    while (schedSpec[i] != ':') {
        i++;
    }
    i++;
    for(; i<n; i++) {
        buffer.push_back(schedSpec[i]);
    }
    return buffer.empty() ? DEFAULT_MAXPRIO : stoi(buffer);
}

static BaseScheduler* getScheduler(char schedSelector, int quantum, int maxPrio) {
    switch(schedSelector) {
        case 'F':
            return new FcfsScheduler(DEFAULT_QUANTUM, DEFAULT_MAXPRIO);
        case 'L':
            return new LcfsScheduler(DEFAULT_QUANTUM, DEFAULT_MAXPRIO);
        case 'S':
            return new SrtfScheduler(DEFAULT_QUANTUM, maxPrio);
        case 'R':
            return new RrScheduler(quantum, maxPrio);
//        case 'P': // TODO: Uncomment
//            return new PreScheduler(quantum, maxPrio);
//        case 'E':
//            return new PrePrioScheduler(quantum, maxPrio);
    }
}

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
    // TODO: output time quantum?

    cout<<schedulerName;
    if(schedulerName=="RR" || schedulerName=="PRIO" || schedulerName=="PREPRIO") {
        cout<<" "<<timeQuantum;
    }
    cout<<endl;

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
