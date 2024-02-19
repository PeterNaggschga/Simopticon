#ifndef SIMOPTICON_MONTECARLO_H
#define SIMOPTICON_MONTECARLO_H

/**
 * @dir
 * In this directory, classes of the Monte Carlo module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the MonteCarlo class is defined.
 */

#include "../Optimizer.h"
#include "../StoppingCondition.h"
#include "nlohmann/json.hpp"

#include <set>
#include <unordered_set>
#include <memory>

class StoppingCondition;

/**
 * This is a simple implementation of Monte Carlo optimization. Parameter values are chosen by random in given bounds.
 * @defgroup montecarlo montecarlo
 * @ingroup optimizer
 * @author Burkhard Hensel
 */

/**
 * A class capable of finding the minimum of a blackbox function using the Monte Carlo algorithm.
 * @ingroup montecarlo
 * @author Burkhard Hensel
 */
class MonteCarlo : public Optimizer {
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
     * Creates a DirectOptimizer that evaluates functions with the given Controller, optimizes the given ParameterDefinition list and stops as defined by the given StoppingCondition.
     * @param ctrl: Controller to be used for evaluating the optimized function.
     * @param params: ParameterDefinition list to be optimized.
     * @param config: Configuration parameters as JSON.
     */
    MonteCarlo(Controller &ctrl, const std::list<std::shared_ptr<ParameterDefinition>> &params, nlohmann::json config);

    /**
     * Starts the optimization using the Monte Carlo algorithm.
     * Only returns when an iteration has completed and #stopCon deems the optimization complete or when #abort was called in the last iteration.
     */
    void runOptimization() override;

    std::string getName() override;

    std::string getStatus() override;

    std::string getStatusBar() override;

};


#endif //SIMOPTICON_MONTECARLO_H
