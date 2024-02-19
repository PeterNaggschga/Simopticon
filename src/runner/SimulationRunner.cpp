/**
 * @file
 * In this file, the implementation of the SimulationRunner class is defined.
 * @author Per Natzschka
 */

#include "SimulationRunner.h"

#include <future>
#include <list>

SimulationRunner::SimulationRunner(unsigned int threads) : Multithreaded(threads) {
}

std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>
SimulationRunner::runSimulations(const std::set<parameterCombination, CmpVectorSharedParameter> &runs) {
    return runMultithreadedFunctions(runs);
}

std::string SimulationRunner::getName() {
    return Status::getName();
}

std::string SimulationRunner::getStatus() {
    return Status::getStatus();
}

std::string SimulationRunner::getStatusBar() {
    return Status::getStatusBar();
}
