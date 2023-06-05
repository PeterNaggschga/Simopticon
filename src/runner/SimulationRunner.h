#ifndef SIMOPTICON_SIMULATIONRUNNER_H
#define SIMOPTICON_SIMULATIONRUNNER_H

#include "../Types.h"
#include "../ComparisonFunctions.h"

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <filesystem>

class Parameter;

using namespace std;

class SimulationRunner {
private:
    const unsigned int NR_THREADS;
    const unsigned int NR_RUNS_PER_THREAD;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulationThread(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) = 0;

public:
    SimulationRunner(unsigned int threads, unsigned int runs);

    virtual ~SimulationRunner() = default;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs);

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
