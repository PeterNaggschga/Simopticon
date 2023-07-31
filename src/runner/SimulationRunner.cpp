#include "SimulationRunner.h"

#include <future>
#include <list>

SimulationRunner::SimulationRunner(unsigned int threads) : Multithreaded(threads) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
SimulationRunner::runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs) {
    return runMultithreadedFunctions(runs);
}

string SimulationRunner::getName() {
    return Status::getName();
}

string SimulationRunner::getStatus() {
    return Status::getStatus();
}

string SimulationRunner::getStatusBar() {
    return Status::getStatusBar();
}
