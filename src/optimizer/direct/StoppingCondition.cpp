/**
 * @file
 * In this file, the implementation of the StoppingCondition class is defined.
 */

#include "StoppingCondition.h"

StoppingCondition::StoppingCondition(size_t evaluations, size_t hyrects, unsigned int minutes,
                                     functionValue accuracy,
                                     unsigned int accuracyIterations)
        : NR_EVALUATIONS(evaluations), NR_HYRECTS(hyrects), mins(minutes), time_eval(minutes != 0), ACCURACY(accuracy),
          NR_ACCURACY_ITERATIONS(accuracyIterations) {
}

/**
 * Helper method, which checks whether the given condition should be used and returns the corresponding value if thats the case.
 * If not, 0 is returned.
 * @tparam T: Type of the value located at the given key.
 * @param object: JSON object the condition is read from.
 * @param key: Key of the fetched condition.
 * @param val: Key of the value field of the fetched condition.
 * @return A value of type T that should be used as value for the condition.
 */
template<typename T>
T getConditionFromJSON(nlohmann::json object, const std::string &key, const std::string &val = "n") {
    return object.at(key).at("useCondition").get<bool>() ? object.at(key).at(val).get<T>() : 0;
}

StoppingCondition::StoppingCondition(nlohmann::json stopCon) :
        NR_EVALUATIONS(getConditionFromJSON<size_t>(stopCon, "evaluations")),
        NR_HYRECTS(getConditionFromJSON<size_t>(stopCon, "hyrects")),
        mins(getConditionFromJSON<unsigned int>(stopCon, "minutes")),
        time_eval(stopCon.at("minutes").at("useCondition").get<bool>()),
        ACCURACY(getConditionFromJSON<functionValue>(stopCon, "accuracy", "delta")),
        NR_ACCURACY_ITERATIONS(getConditionFromJSON<unsigned int>(stopCon, "accuracy")) {
}


bool StoppingCondition::evaluate(size_t evaluations, size_t hyrects, functionValue newBestVal) {
    bool eval = NR_EVALUATIONS == 0 || evaluations < NR_EVALUATIONS;
    bool rects = NR_HYRECTS == 0 || hyrects < NR_HYRECTS;
    bool time = !time_eval || END_TIME > std::chrono::system_clock::now();
    bool accuracy = updateAccuracy(newBestVal) || (ACCURACY == 0 && NR_ACCURACY_ITERATIONS == 0);
    return eval && rects && time && accuracy;
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
