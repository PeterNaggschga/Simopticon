#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H


#include "../Types.h"
#include "../ComparisonFunctions.h"
#include "../optimizer/Optimizer.h"
#include "../runner/SimulationRunner.h"
#include "../evaluation/Evaluation.h"
#include "../parameters/Parameter.h"
#include "../status/StatusBar.h"
#include "../utils/Abortable.h"

#include <map>
#include <vector>
#include <list>
#include <memory>
#include <set>

class ParameterDefinition;

class ValueMap;

using namespace std;

/**
 * This module provides classes coordinating the optimization process independently from the actual implementation of Optimizer, SimulationRunner and Evaluation.
 * @defgroup controller controller
 */

/**
 * A class responsible for communication between Optimizer, SimulationRunner and Evaluation and also
 * user interaction such as tracking results, updating StatusBar and handling interrupts by the user via Abortable.
 * @ingroup controller
 */
class Controller : public Abortable {
private:
    /**
     * Defines if result files of best simulations are kept after optimization. Can be set in main config.
     */
    bool keepFiles;
    /**
     * Saves the best @a n Parameter combinations and the corresponding path to the result files, if #keepFiles is true. @a n can be set in main config.
     */
    map<vector<shared_ptr<Parameter>>, filesystem::path> topResults;

    /**
     * Interval of updates of StatusBar using #updateStatus in concurrent status thread.
     */
    chrono::milliseconds statusInterval = chrono::milliseconds(0);

    /**
     * Calls the #runner to run simulations for the given Parameter combinations.
     * Updates #statusBar before and after execution of simulations.
     * @param runs: A set of Parameter combinations to be executed.
     * @return A map which maps the given Parameter combinations to their respective result file paths and runIds.
     */
    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs);

    /**
     * Calls the #evaluation to evaluate the given result files.
     * Updates #statusBar before and after execution of evaluation.
     * @param simulationResults: A map which maps the Parameter combinations that must be evaluated to their respective file paths of simulation results and runIds.
     * @return A map which maps the given Parameter combinations to their respective functionValue.
     */
    virtual map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> evaluate(
            const map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> &simulationResults);

    /**
     * Removes all result files that don't belong to the best @a n results, where @a n is configured in main config.
     * If #keepFiles is @a false, all result files are removed.
     */
    virtual void removeOldResultfiles();

    /**
     * Updates the #statusBar using StatusBar#updateStatus.
     */
    virtual void updateStatus();

protected:
    /**
     * StatusBar object used for output.
     */
    StatusBar statusBar;
    /**
     * Optimizer defining an optimization strategy.
     */
    unique_ptr<Optimizer> optimizer;
    /**
     * SimulationRunner able to run simulations with certain Parameter combinations.
     */
    unique_ptr<SimulationRunner> runner;
    /**
     * Evaluation capable of evaluating data produced by #runner.
     */
    unique_ptr<Evaluation> evaluation;
    /**
     * ValueMap containing all values gathered by simulating and evaluating certain Parameter combinations.
     */
    unique_ptr<ValueMap> valueMap;

/**
 * A struct keeping track of the currently running optimization step for StatusBar#updateStatus.
 * @ingroup controller
 */
    struct stepstate {
        /**
         * Defines if #currentStep has changed since the last call to #get.
         */
        bool stepChanged;
        /**
         * Current step the optimization is in.
         */
        step currentStep = INIT;

        /**
         * Switches #currentStep to the next step.
         */
        void next() {
            stepChanged = true;
            currentStep = static_cast<step>((currentStep + 1) % 3);
        }

        /**
         * Returns the value of #currentStep.
         * @return The step that is currently run.
         */
        step get() {
            stepChanged = false;
            return currentStep;
        }
    }
/**
 * An object keeping track of the current optimization step.
 */
    stepState;

public:
    /**
     * Creates a Controller which uses Optimizer, SimulationRunner and Evaluation as specified in the given config files.
     * If called by the constructor of StubController, #runner and #evaluation get assigned null-pointers.
     * @param configPath: Path to the main config. Chosen by first command line argument.
     * @param isStub: Defines whether the constructor was called by constructor of StubController.
     */
    explicit Controller(const filesystem::path &configPath, bool isStub = false);

    /**
     * Starts optimization process by calling Optimizer#runOptimization.
     * Creates concurrent thread that updates #statusBar every #statusInterval milliseconds.
     * Prints results in command line after optimization is done using StatusBar#printResults.
     */
    void run();

    /**
     * Searches #valueMap for results to given Parameter combinations.
     * Each combination that hasn't been simulated is simulated and evaluated using #runSimulations and #evaluate.
     * Updates #statusBar before and after execution.
     * @param params: A set of Parameter combinations to be evaluated.
     * @return A map which maps the given Parameter combinations to their respective functionValue.
     */
    map<vector<shared_ptr<Parameter>>, functionValue> requestValues(const list<vector<shared_ptr<Parameter>>> &params);

    /**
     * Returns #valueMap.
     * @return A ValueMap object.
     */
    [[nodiscard]] ValueMap &getValueMap();

    /**
     * Aborts #optimizer using Optimizer#abort.
     * Aborts the concurrent thread that regularly updates #statusBar.
     */
    void abort() override;
};


#endif //SIMOPTICON_CONTROLLER_H
