#ifndef SIMOPTICON_PLEXESIMULATIONRUNNER_H
#define SIMOPTICON_PLEXESIMULATIONRUNNER_H


#include "../SimulationRunner.h"

#include <list>
#include <mutex>

class PlexeSimulationRunner : public SimulationRunner {
private:
    const unsigned int REPEAT;
    const vector<string> SCENARIOS;

    unsigned long runNumber = 0;
    mutex runNumberLock;

    unsigned long getRunId();

    map<vector<shared_ptr<Parameter>>, runId, CmpVectorSharedParameter>
    runSimulationThread(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) override;

public:
    PlexeSimulationRunner(unsigned int threads, unsigned int runs, unsigned int repeat, vector<string> scenarios);

};


#endif //SIMOPTICON_PLEXESIMULATIONRUNNER_H
