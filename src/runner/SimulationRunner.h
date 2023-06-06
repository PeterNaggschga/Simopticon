#ifndef SIMOPTICON_SIMULATIONRUNNER_H
#define SIMOPTICON_SIMULATIONRUNNER_H

#include "../Types.h"
#include "../ComparisonFunctions.h"

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <filesystem>
#include <semaphore>
#include <queue>

class Parameter;

using namespace std;

class SimulationRunner {
protected:
    const static unsigned int SEMAPHORE_MAX = UINT16_MAX;

    set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> getNextRun();

private:
    const unsigned int NR_THREADS;
    const unsigned int NR_RUNS_PER_THREAD;

    queue<set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter>> runQueue;
    mutex runQueueLock;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulationThread() = 0;

public:
    SimulationRunner(unsigned int threads, unsigned int runs);

    virtual ~SimulationRunner() = default;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs);

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
