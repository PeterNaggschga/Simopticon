#ifndef SIMOPTICON_SIMULATIONRUNNER_H
#define SIMOPTICON_SIMULATIONRUNNER_H

/**
 * @dir
 * In this directory, classes of the runner module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the SimulationRunner class is defined.
 */

#include "../Types.h"
#include "../ComparisonFunctions.h"
#include "../utils/Multithreaded.h"
#include "../status/Status.h"

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <filesystem>
#include <queue>

class Parameter;

/**
 * This module contains components capable of automatically running simulations with certain parameterCombinations.
 * Implementations must extend SimulationRunner.
 * @defgroup runner runner
 */

/**
 * A class capable of running simulations with certain parameterCombinations.
 * @ingroup runner
 * @author Per Natzschka
 */
class SimulationRunner :
        public Multithreaded<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>,
        public Status {
private:
    /**
     * Deals with the simulation of a single parameterCombination.
     * Overrides Multithreaded#work and therefore can be executed concurrently.
     * @param run: parameterCombination to be simulated.
     * @return A pair containing a path to the result directory and a set of runIds identifying the respective simulation runs.
     */
    std::pair<std::filesystem::path, std::set<runId>> work(parameterCombination run) override = 0;

public:
    /**
     * Creates a SimulationRunner which can use no more than the given number of threads to simulate parameterCombinations concurrently.
     * @param threads: Maximum number of threads that may be used for concurrent simulations.
     */
    explicit SimulationRunner(unsigned int threads);

    virtual ~SimulationRunner() = default;

    /**
     * Simulates the given parameterCombinations concurrently and returns their respective results.
     * Basically calls Multithreaded#runMultithreadedFunctions which uses the ThreadPool pattern to parallelize the execution of #work.
     * @param runs: Set of parameterCombinations to be simulated.
     * @return A map which maps the given parameterCombinations to their respective result directory and runIds.
     */
    virtual std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>
    runSimulations(const std::set<parameterCombination, CmpVectorSharedParameter> &runs);

    std::string getName() override;

    std::string getStatus() override;

    std::string getStatusBar() override;

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
