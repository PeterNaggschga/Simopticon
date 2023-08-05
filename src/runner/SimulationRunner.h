#ifndef SIMOPTICON_SIMULATIONRUNNER_H
#define SIMOPTICON_SIMULATIONRUNNER_H

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

using namespace std;

/**
 * This module contains components capable of automatically running simulations with certain Parameter combinations.
 * Implementations must extend SimulationRunner.
 * @defgroup runner runner
 */

/**
 * A class capable of running simulations with certain Parameter combinations.
 * @ingroup runner
 */
class SimulationRunner
        : public Multithreaded<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>,
          public Status {
private:
    /**
     * Deals with the simulation of a single Parameter combination.
     * Overrides Multithreaded#work and therefore can be executed concurrently.
     * @param run: Parameter combination to be simulated.
     * @return A pair containing a path to the result directory and a set of runIds identifying the respective simulation runs.
     */
    pair<filesystem::path, set<runId>> work(vector<shared_ptr<Parameter>> run) override = 0;

public:
    /**
     * Creates a SimulationRunner which can use no more than the given number of threads to simulate Parameter combinations concurrently.
     * @param threads: Maximum number of threads that may be used for concurrent simulations.
     */
    explicit SimulationRunner(unsigned int threads);

    virtual ~SimulationRunner() = default;

    /**
     * Simulates the given Parameter combinations concurrently and returns their respective results.
     * Basically calls Multithreaded#runMultithreadedFunctions which uses the ThreadPool pattern to parallelize the execution of #work.
     * @param runs: Set of Parameter combinations to be simulated.
     * @return A map which maps the given Parameter combinations to their respective result directory and runIds.
     */
    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs);

    string getName() override;

    string getStatus() override;

    string getStatusBar() override;

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
