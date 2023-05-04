#ifndef SIMOPTICON_SIMULATIONRUNNER_H
#define SIMOPTICON_SIMULATIONRUNNER_H

#include <string>
#include <vector>
#include "../parameters/Parameter.h"

using namespace std;

class SimulationRunner {
public:
    virtual string runSimulation(vector<Parameter>) = 0;

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
