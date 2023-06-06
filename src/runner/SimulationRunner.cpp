#include "SimulationRunner.h"

#include <cmath>
#include <future>
#include <list>

SimulationRunner::SimulationRunner(unsigned int threads, unsigned int runs) : NR_THREADS(threads),
                                                                              NR_RUNS_PER_THREAD(runs) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
SimulationRunner::runSimulations(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) {
    size_t nrRuns = runs.size();
    for (int i = 0; i < ceil((double) nrRuns / NR_RUNS_PER_THREAD); ++i) {
        auto it = runs.begin();
        advance(it, NR_RUNS_PER_THREAD < runs.size() ? NR_RUNS_PER_THREAD : runs.size());
        runQueue.emplace(runs.begin(), it);
        runs.erase(runs.begin(), it);
    }

    list<future<map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>>> threads;
    for (int i = 0; i < min((size_t) NR_THREADS, nrRuns); ++i) {
        threads.push_back(async(std::launch::async, &SimulationRunner::runSimulationThread, this));
    }

    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> result;
    for (auto &entry: threads) {
        entry.wait();
        result.merge(entry.get());
    }

    return result;
}

set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> SimulationRunner::getNextRun() {
    runQueueLock.lock();
    if (runQueue.empty()) {
        runQueueLock.unlock();
        return {};
    }
    auto result = runQueue.front();
    runQueue.pop();
    runQueueLock.unlock();
    return result;
}
