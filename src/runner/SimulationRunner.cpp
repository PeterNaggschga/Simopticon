#include "SimulationRunner.h"

#include <cmath>

SimulationRunner::SimulationRunner(unsigned int threads, unsigned int runs) : NR_THREADS(threads),
                                                                              NR_RUNS_PER_THREAD(runs) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
SimulationRunner::runSimulations(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) {
    unsigned int nrThreadRuns = ceil((double) runs.size() / NR_RUNS_PER_THREAD);
    set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> threadRuns[nrThreadRuns];
    for (int i = 0; i < nrThreadRuns; ++i) {
        auto it = runs.begin();
        advance(it, NR_RUNS_PER_THREAD < runs.size() ? NR_RUNS_PER_THREAD : runs.size());
        threadRuns[i].insert(runs.begin(), it);
        runs.erase(runs.begin(), it);
    }

    // TODO: Multithreading
    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> result;
    for (const auto &threadRun: threadRuns) {
        auto threadResult = runSimulationThread(threadRun);
        result.insert(threadResult.begin(), threadResult.end());
    }


    return result;
}
