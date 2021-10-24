#include "RNG.h"

void RNG::readRandomNumberFile(string &randomFilePath) {
    ifstream infile(randomFilePath);
    string line;
    getline(infile, line);
    n = stoi(line);
    for(int i=0; i<n; i++) {
        getline(infile, line);
        rnos.push_back(stoi(line));
    }
}

RNG::RNG(string &randomFilePath) {
    readRandomNumberFile(randomFilePath); // Read in the random numbers file
    randSize = rnos.size();
    ofs = 0;
}

int RNG::random(int burst) {
    int ret = 1 + (rnos[ofs] % burst);
    ofs = (ofs + 1) % randSize;
    return ret;
}
