#ifndef SIMOPTICON_RANDOMNEIGHBORS_H
#define SIMOPTICON_RANDOMNEIGHBORS_H

/**
 * @dir
 * In this directory, classes of the RandomNeighbors module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the RandomNeighbors optimizer class is defined.
 */

#include "../Optimizer.h"
#include "../StoppingCondition.h"
#include "nlohmann/json.hpp"

#include <set>
#include <unordered_set>
#include <memory>

class StoppingCondition;

/**
 * This is a simple implementation of Random Neighbors optimization. Parameter values are chosen by random in given bounds.
 * Besides global search, it is searched more oftern in the neighbourbood of the current optimum.
 *
 * @defgroup randomneighbors randomneighbors
 * @ingroup optimizer
 */

/**
 * A class capable of finding the minimum of a blackbox function using the Random Neighbors algorithm.
 * @ingroup randomneighbors
 * @author Burkhard Hensel
 */
class RandomNeighbors : public Optimizer {
private:
    /**
     * Number of iterations completed.
     */
    size_t iterations = 1;
    /**
     * Number of (typically parallel executed) parameter combinations per iteration.
     */
    const size_t parallelTrials;
    /**
     * The search width around the current optimum relative to each parameter bounds pair.
     */
    const double neighborhoodWidth;
    /**
     * The probability for searching in the neighborhood instead of globally.
     */
    const double localSearchProbability;
    /**
     * True, if the last iteration was a local search, false if global search
     */
    bool lastLocal = false;
    /**
     * An object deciding when the optimization stops.
     */
    StoppingCondition stopCon;
    /**
     * Defines if the current number of iterations, evaluations, rectangles and the optimal value should be recorded into a `.csv` file after each iteration.
     * Can be set in config.
     */
    bool trackProgress;
    /**
     * A string for logging purposes containing the evaluation of the simulation runs of the last iteration.
     */
    std::string lastEvaluations;

    /**
     * Prints the current number of iterations, evaluations, rectangles and the current optimal value to a `.csv` file.
     * @param bestVal: Value at the current minimum.
     * @param evaluations: Number of evaluations conducted by the optimization.
     */
    void saveProgress(functionValue bestVal, size_t evaluations) const;

public:
    /**
     * Creates a RandomNeighbors optimizer that evaluates functions with the given Controller, optimizes the given ParameterDefinition list and stops as defined by the given StoppingCondition.
     * @param ctrl: Controller to be used for evaluating the optimized function.
     * @param params: ParameterDefinition list to be optimized.
     * @param config: Configuration parameters as JSON.
     */
    RandomNeighbors(Controller &ctrl, const std::list<std::shared_ptr<ParameterDefinition>> &params,
                    nlohmann::json config);

    /**
     * Starts the optimization.
     * Only returns when an iteration has completed and #stopCon deems the optimization complete or when #abort was called in the last iteration.
     */
    void runOptimization() override;

    std::string getName() override;

    std::string getStatus() override;

    std::string getStatusBar() override;

};


#endif //SIMOPTICON_RANDOMNEIGHBORS_H
