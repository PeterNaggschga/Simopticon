/**
 * @file
 * In this file, the implementation of the StoppingCondition class is defined.
 * @author Per Natzschka
 */

#include "StoppingCondition.h"

StoppingCondition::StoppingCondition(size_t evaluations, unsigned int minutes,
                                     functionValue accuracy,
                                     unsigned int accuracyIterations)
        : NR_EVALUATIONS(evaluations), mins(minutes), time_eval(minutes != 0), ACCURACY(accuracy),
          NR_ACCURACY_ITERATIONS(accuracyIterations) {
}

StoppingCondition::StoppingCondition(nlohmann::json stopCon) :
        NR_EVALUATIONS(getConditionFromJSON<size_t>(stopCon, "evaluations")),
        mins(getConditionFromJSON<unsigned int>(stopCon, "minutes")),
        time_eval(stopCon.at("minutes").at("useCondition").get<bool>()),
        ACCURACY(getConditionFromJSON<functionValue>(stopCon, "accuracy", "delta")),
        NR_ACCURACY_ITERATIONS(getConditionFromJSON<unsigned int>(stopCon, "accuracy")) {
}


bool StoppingCondition::evaluate(size_t evaluations, functionValue newBestVal) {
    bool eval = NR_EVALUATIONS == 0 || evaluations < NR_EVALUATIONS;
    bool time = !time_eval || END_TIME > std::chrono::system_clock::now();
    bool accuracy = updateAccuracy(newBestVal) || (ACCURACY == 0 && NR_ACCURACY_ITERATIONS == 0);
    return eval && time && accuracy;
}

void StoppingCondition::setStartNow() {
    if (time_eval) {
        END_TIME = std::chrono::time_point_cast<std::chrono::seconds>(
                std::chrono::system_clock::now() + std::chrono::minutes(mins));
    }
}

bool StoppingCondition::updateAccuracy(functionValue newBestVal) {
    if (newBestVal < bestVal - ACCURACY) {
        iterationsSinceImprov = 0;
        bestVal = newBestVal;
    } else {
        iterationsSinceImprov++;
    }
    return iterationsSinceImprov < NR_ACCURACY_ITERATIONS;
}

unsigned int StoppingCondition::getIterationsSinceImprov() const {
    return iterationsSinceImprov;
}
