#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Process.h"

using namespace std;

static vector<Process*> readProcessFile(string &processFilePath) {
    vector<Process*> processes;
    ifstream infile(processFilePath);
    string line;
    int currentPid = 0;

    while(getline(infile, line)){   // read a line
        vector<int> processParams;
        stringstream ss(line);

        while(getline(ss, line, ' ')) { // split on spaces
            processParams.push_back(stoi(line));
        }
        if(processParams.size() != 4) {
            break;
        }
        processes.push_back(
                new Process(
                        currentPid, // TODO: Check whether pids are being allocated and incremented correctly
                        processParams[0],
                        processParams[1],
                        processParams[2],
                        processParams[3]
                )
        );
        currentPid++;
    }

    return processes;
}
