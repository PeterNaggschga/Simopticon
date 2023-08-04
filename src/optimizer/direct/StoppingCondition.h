#ifndef SIMOPTICON_STOPPINGCONDITION_H
#define SIMOPTICON_STOPPINGCONDITION_H


#include "../../Types.h"
#include "nlohmann/json.hpp"

#include <chrono>
#include <cmath>

using namespace std::chrono;
using json = nlohmann::json;

/**
 * A class used for deciding whether the DIRECT should be stopped.
 * Every conditions is optional and can be set in config. The optimization is stopped when one of the activated conditions is met.
 * @ingroup direct
 */
class StoppingCondition {
private:
    /**
     * Number of evaluations after which the optimization should stop.
     */
    const size_t NR_EVALUATIONS;
    /**
     * Number of rectangles in the partition after which the optimization should stop.
     */
    const size_t NR_HYRECTS;

    /**
     * Point in time after which optimization should end.
     * Calculated using time when #setStartNow is called and #mins.
     */
    time_point<system_clock, seconds> END_TIME;
    /**
     * Number of minutes after which the optimization should stop.
     */
    const unsigned int mins;
    /**
     * Defines whether the time condition should be used.
     */
    bool time_eval;

    /**
     * Accuracy used in accuracy condition.
     * When the #bestVal has not changed more than #ACCURACY after #NR_ACCURACY_ITERATIONS iterations, the optimization is stopped.
     */
    const functionValue ACCURACY;
    /**
     * Number of iterations used in accuracy condition.
     * When the #bestVal has not changed more than #ACCURACY after #NR_ACCURACY_ITERATIONS iterations, the optimization is stopped.
     */
    const unsigned int NR_ACCURACY_ITERATIONS;
    /**
     * Best value used to keep track of accuracy condition.
     * When the #bestVal has not changed more than #ACCURACY after #NR_ACCURACY_ITERATIONS iterations, the optimization is stopped.
     */
    functionValue bestVal = INFINITY;
    /**
     * Number of iterations since last improvement of the optimum used to keep track of accuracy condition.
     * When the #bestVal has not changed more than #ACCURACY after #NR_ACCURACY_ITERATIONS iterations, the optimization is stopped.
     */
    unsigned int iterationsSinceImprov = 0;

    /**
     * Checks if the current optimum improves the one saved in #bestVal by more than #ACCURACY.
     * If that is the case, #iterationsSinceImprov is reset to zero and the current optimum is saved in #bestVal.
     * If not #iterationsSinceImprov is increased.
     * @param newBestVal: Current optimum.
     * @return A bool defining if the accuracy condition is met after the values where updated.
     */
    bool updateAccuracy(functionValue newBestVal);

public:
    /**
     * Creates a StoppingCondition with the given condition values.
     * @param evaluations: Number of evaluations after which the optimization should stop.
     * @param hyrects: Number of rectangles in the partition after which the optimization should stop.
     * @param minutes: Number of minutes after which the optimization should stop.
     * @param accuracy: Accuracy used in accuracy condition (see #ACCURACY).
     * @param accuracyIterations: Number of iterations used in accuracy condition (see #NR_ACCURACY_ITERATIONS).
     */
    explicit StoppingCondition(size_t evaluations = 0, size_t hyrects = 0, unsigned int minutes = 0,
                               functionValue accuracy = 0, unsigned int accuracyIterations = 0);

    /**
     * Creates a StoppingCondition based on the given json configuration.
     * @param stopCon: JSON object defining the condition values.
     */
    explicit StoppingCondition(json stopCon);

    /**
     * Sets #END_TIME to be the current time plus #mins.
     */
    void setStartNow();

    /**
     * Checks if any of the configured conditions is met for the given parameters.
     * @param evaluations: Number of evaluations conducted by the optimization.
     * @param hyrects: Number of rectangles in the current partition.
     * @param newBestVal: Value of the current optimum.
     * @return A boolean defining whether none of the configured conditions is met (meaning whether the optimization should keep running).
     */
    bool evaluate(size_t evaluations, size_t hyrects, functionValue newBestVal);

    /**
     * Returns the value of #iterationsSinceImprov.
     * @return An integral representing the number of iterations since the best value improved by more than #ACCURACY.
     */
    [[nodiscard]] unsigned int getIterationsSinceImprov() const;
};


#endif //SIMOPTICON_STOPPINGCONDITION_H
