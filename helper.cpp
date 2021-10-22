#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Process.cpp"

using namespace std;

static vector<Process*> readProcessFile(string &processFilePath) {
    vector<Process*> processes;
    ifstream infile(processFilePath);
    string line;

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
                        processParams[0],
                        processParams[1],
                        processParams[2],
                        processParams[3]
                )
        );
    }

    return processes;
}
