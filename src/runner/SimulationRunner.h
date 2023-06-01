#ifndef SIMOPTICON_SIMULATIONRUNNER_H
#define SIMOPTICON_SIMULATIONRUNNER_H

#include <string>
#include <vector>

class Parameter;

using namespace std;

class SimulationRunner {
public:
    virtual ~SimulationRunner() = default;

    virtual string runSimulation(vector<Parameter>) = 0;

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
