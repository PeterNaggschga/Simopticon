/**
 * @file
 * In this file, the implementation of the SimulationRunner class is defined.
 */

#include "SimulationRunner.h"

#include <future>
#include <list>

SimulationRunner::SimulationRunner(unsigned int threads) : Multithreaded(threads) {
}

map<parameterCombination, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
SimulationRunner::runSimulations(const set<parameterCombination, CmpVectorSharedParameter> &runs) {
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
