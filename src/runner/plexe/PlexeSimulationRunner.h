#ifndef SIMOPTICON_PLEXESIMULATIONRUNNER_H
#define SIMOPTICON_PLEXESIMULATIONRUNNER_H


#include "../SimulationRunner.h"
#include "ConfigEditor.h"

#include <list>
#include <mutex>

class PlexeSimulationRunner : public SimulationRunner {
private:
    const unsigned int REPEAT;
    const vector<string> SCENARIOS;

    ConfigEditor editor;

    size_t runNumber = 0;
    mutex runNumberLock;

    size_t getRunId();

    map<vector<shared_ptr<Parameter>>, runId, CmpVectorSharedParameter>
    runSimulationThread(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) override;

public:
    PlexeSimulationRunner(unsigned int threads, unsigned int runs, unsigned int repeat, vector<string> scenarios,
                          ConfigEditor editor);

};


#endif //SIMOPTICON_PLEXESIMULATIONRUNNER_H
