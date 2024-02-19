#ifndef SIMOPTICON_DIRECTSTOPPINGCONDITION_H
#define SIMOPTICON_DIRECTSTOPPINGCONDITION_H

/**
 * @file
 * In this file, the header of the StoppingCondition class is defined.
 */

#include "../../Types.h"
#include "nlohmann/json.hpp"
#include "../StoppingCondition.h"

#include <chrono>
#include <cmath>

/**
 * A class used for deciding whether the DIRECT should be stopped.
 * Every conditions is optional and can be set in config. The optimization is stopped when one of the activated conditions is met.
 * @ingroup direct
 */
class DirectStoppingCondition : public StoppingCondition {
private:
    /**
     * Number of rectangles in the partition after which the optimization should stop.
     */
    const size_t NR_HYRECTS;


public:
    /**
     * Creates a StoppingCondition with the given condition values.
     * @param evaluations: Number of evaluations after which the optimization should stop.
     * @param hyrects: Number of rectangles in the partition after which the optimization should stop.
     * @param minutes: Number of minutes after which the optimization should stop.
     * @param accuracy: Accuracy used in accuracy condition (see #ACCURACY).
     * @param accuracyIterations: Number of iterations used in accuracy condition (see #NR_ACCURACY_ITERATIONS).
     */
    explicit DirectStoppingCondition(size_t evaluations = 0, size_t hyrects = 0, unsigned int minutes = 0,
                                     functionValue accuracy = 0, unsigned int accuracyIterations = 0);

    /**
     * Creates a StoppingCondition based on the given json configuration.
     * @param stopCon: JSON object defining the condition values.
     */
    explicit DirectStoppingCondition(const nlohmann::json &stopCon);

    /**
     * Checks if any of the configured conditions is met for the given parameters.
     * @param evaluations: Number of evaluations conducted by the optimization.
     * @param hyrects: Number of rectangles in the current partition.
     * @param newBestVal: Value of the current optimum.
     * @return A boolean defining whether none of the configured conditions is met (meaning whether the optimization should keep running).
     */
    bool evaluate(size_t evaluations, size_t hyrects, functionValue newBestVal);

};


#endif //SIMOPTICON_DIRECTSTOPPINGCONDITION_H
