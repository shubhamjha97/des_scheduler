#include <iostream>
#include "RNG.cpp"
#include "helper.cpp"

using namespace std;

int main() {
    // Initialize the RNG
    RNG rng = RNG("./rfile");

    // Read input file
    string PROCESS_FILE_PATH = "/Users/sjha/Downloads/lab2_assign/input1";
    vector<Process*> processes = readProcessFile(PROCESS_FILE_PATH);

    return 0;
}
