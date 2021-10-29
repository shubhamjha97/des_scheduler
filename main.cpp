#include <iostream>
#include <getopt.h>
#include "RNG.h"
#include "helper.h"
#include "DES.cpp"
#include "Event.h"

using namespace std;

int main(int argc, char **argv) {
    int c;
    bool vflag, tflag, eflag, pflag; // Throw away these flags
    string schedSpec;

    while ((c = getopt (argc, argv, "vteps:")) != -1) {
        switch (c) {
            case 'v':
                vflag = 1;
                break;
            case 't':
                tflag = 1;
                break;
            case 'e':
                eflag = 1;
                break;
            case 'p':
                pflag = 1;
                break;
            case 's':
                schedSpec = optarg;
                break;
            case '?':
                printf("Unknown option character: %c", c);
        }
    }

    string PROCESS_FILE_PATH =  argv[optind++];
    string RANDOM_FILE_PATH = argv[optind++];

    int quantum = parseForQuantum(schedSpec);
    int maxPrio = parseForMaxPrio(schedSpec);

    // Initialize the RNG
    RNG *rng = new RNG(RANDOM_FILE_PATH);

    // Get the scheduler depending on the schedSpec
    BaseScheduler *scheduler = getScheduler(schedSpec[0], quantum, maxPrio);

    // Initialize the Discrete Event Simulator
    DES des = DES(scheduler, rng);

    // Read input file
    vector<Process*> processes = readProcessFile(PROCESS_FILE_PATH, maxPrio, rng);

    // Add the loaded processes into the DES
    for(auto process : processes) {
        des.putEvent(new Event(process, process->arrivalTime, CREATED, TRANS_TO_READY, 0));
    }

    // Run the simulation
    des.runSimulation();

    dumpResultsToConsole(scheduler->getSchedulerName(), quantum, processes);

    return 0;
}
