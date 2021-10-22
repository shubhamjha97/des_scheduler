#include <vector>
#include <string>
#include <fstream>

using namespace std;

class RNG {

private:
    vector<int> rnos;
    int randSize, ofs;

    void readRandomNumberFile(string randomFilePath) {
        ifstream infile(randomFilePath);
        string line;
        while (getline(infile, line)) {
            rnos.push_back(stoi(line));
        }
    }

public:
    RNG(string randomFilePath) {
        readRandomNumberFile(randomFilePath); // Read in the random numbers file
        randSize = rnos.size();
        ofs = 0;
    }

    int myrandom(int burst) {
        int ret = 1 + (rnos[ofs] % burst);
        ofs = (ofs + 1) % randSize;
        return ret;
    }
};
