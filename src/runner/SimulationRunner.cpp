#include "SimulationRunner.h"

#include <future>
#include <list>

SimulationRunner::SimulationRunner(unsigned int threads) : NR_THREADS(threads) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
SimulationRunner::runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs) {
    for (const auto &entry: runs) {
        runQueue.push(entry);
    }

    list<future<map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>>> threads;
    for (int i = 0; i < min((size_t) NR_THREADS, runs.size()); ++i) {
        threads.push_back(async(std::launch::async, &SimulationRunner::runSimulationThread, this));
    }

    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> result;
    for (auto &entry: threads) {
        entry.wait();
        result.merge(entry.get());
    }

    return result;
}

vector<shared_ptr<Parameter>> SimulationRunner::getNextRun() {
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
