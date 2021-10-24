#ifndef SCHEDULER_RNG_H
#define SCHEDULER_RNG_H

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class RNG {
private:
    vector<int> rnos;
    int randSize, ofs, n;
    void readRandomNumberFile(string &randomFilePath);

public:
    explicit RNG(string &randomFilePath);
    int random(int burst);
};

#endif //SCHEDULER_RNG_H
