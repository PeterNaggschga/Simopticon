#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H

/**
 * @dir
 * In this directory, classes of the controller module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the Controller class is defined.
 */

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
     * Defines if all found values should be recorded in a `.csv` file after optimization has finished.
     * Can be set in main config.
     */
    bool printValues;
    /**
     * Saves the best @a n parameterCombinations and the corresponding path to the result files, if #keepFiles is true. @a n can be set in main config.
     */
    std::map<parameterCombination, std::filesystem::path> topResults;
    /**
     * Interval of updates of StatusBar using #updateStatus in concurrent status thread.
     */
    std::chrono::milliseconds statusInterval = std::chrono::milliseconds(0);

    /**
     * Calls the #runner to run simulations for the given parameterCombinations.
     * Updates #statusBar before and after execution of simulations.
     * @param runs: A set of parameterCombinations to be executed.
     * @return A map which maps the given parameterCombinations to their respective result file paths and runIds.
     */
    virtual std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>
    runSimulations(const std::set<parameterCombination, CmpVectorSharedParameter> &runs);

    /**
     * Calls the #evaluation to evaluate the given result files.
     * Updates #statusBar before and after execution of evaluation.
     * @param simulationResults: A map which maps the parameterCombinations that must be evaluated to their respective file paths of simulation results and runIds.
     * @return A map which maps the given parameterCombinations to their respective functionValue.
     */
    virtual std::map<parameterCombination, functionValue, CmpVectorSharedParameter> evaluate(
            const std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter> &simulationResults);

    /**
     * Removes all result files that don't belong to the best @a n results, where @a n is configured in main config.
     * If #keepFiles is @a false, all result files are removed.
     */
    virtual void removeOldResultfiles();

    /**
     * Prints all evaluated parameterCombinations and their respective values to `results/values.csv`.
     */
    void saveValues();

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
    std::unique_ptr<Optimizer> optimizer;
    /**
     * SimulationRunner able to run simulations with certain parameterCombinations.
     */
    std::unique_ptr<SimulationRunner> runner;
    /**
     * Evaluation capable of evaluating data produced by #runner.
     */
    std::unique_ptr<Evaluation> evaluation;
    /**
     * ValueMap containing all values gathered by simulating and evaluating certain parameterCombinations.
     */
    std::unique_ptr<ValueMap> valueMap;

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
    explicit Controller(const std::filesystem::path &configPath, bool isStub = false);

    /**
     * Starts optimization process by calling Optimizer#runOptimization.
     * Creates concurrent thread that updates #statusBar every #statusInterval milliseconds.
     * Prints results in command line after optimization is done using StatusBar#printResults.
     */
    void run();

    /**
     * Searches #valueMap for results to given parameterCombinations.
     * Each combination that hasn't been simulated is simulated and evaluated using #runSimulations and #evaluate.
     * Updates #statusBar before and after execution.
     * @param params: A set of parameterCombinations to be evaluated.
     * @return A map which maps the given parameterCombinations to their respective functionValue.
     */
    std::map<parameterCombination, functionValue> requestValues(const std::list<parameterCombination> &params);

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
