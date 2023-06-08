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
    vector<shared_ptr<Parameter>> getNextRun();

private:
    const unsigned int NR_THREADS;

    queue<vector<shared_ptr<Parameter>>> runQueue;
    mutex runQueueLock;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulationThread();

    virtual pair<filesystem::path, set<runId>> executeSimulation(vector<shared_ptr<Parameter>> run) = 0;

public:
    explicit SimulationRunner(unsigned int threads);

    virtual ~SimulationRunner() = default;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(
            const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs);

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
