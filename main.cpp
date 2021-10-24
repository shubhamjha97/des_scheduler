#include <iostream>
#include "RNG.h"
#include "helper.h"
#include "DES.cpp"
#include "Event.h"
#include "FcfsScheduler.cpp"

using namespace std;

int main() {

    // TODO: set up getopt

    // Initialize the RNG
    string RANDOM_FILE_PATH = "./rfile";
    RNG *rng = new RNG(RANDOM_FILE_PATH);

    string schedulerName = "FCFS";
    int timeQuantum = 10000;
    BaseScheduler *scheduler = new FcfsScheduler();

    // Initialize the Discrete Event Simulator
    DES des = DES(scheduler, rng);

    // Read input file
    string PROCESS_FILE_PATH = "/Users/sjha/Downloads/lab2_assign/input1";
    vector<Process*> processes = readProcessFile(PROCESS_FILE_PATH, 4, rng); // TODO: Read maxprio from args

    // Add the loaded processes into the DES
    for(auto process : processes) {
        des.putEvent(new Event(process, process->arrivalTime, CREATED, TRANS_TO_READY));
    }

    // Run the simulation
    des.runSimulation();

    // TODO: Calculate and dump results
    dumpResultsToConsole(schedulerName, timeQuantum, processes);

    return 0;
}
