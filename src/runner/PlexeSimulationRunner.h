#ifndef SIMOPTICON_PLEXESIMULATIONRUNNER_H
#define SIMOPTICON_PLEXESIMULATIONRUNNER_H


#include "SimulationRunner.h"

class PlexeSimulationRunner : public SimulationRunner {
public:
    string runSimulation(vector<Parameter>) override;

};


#endif //SIMOPTICON_PLEXESIMULATIONRUNNER_H
