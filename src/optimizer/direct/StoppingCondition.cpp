#include "StoppingCondition.h"

StoppingCondition::StoppingCondition(size_t evaluations, size_t hyrects, unsigned int minutes,
                                     functionValue accuracy,
                                     unsigned int accuracyIterations)
        : NR_EVALUATIONS(evaluations), NR_HYRECTS(hyrects), mins(minutes), time_eval(minutes != 0), ACCURACY(accuracy),
          NR_ACCURACY_ITERATIONS(accuracyIterations) {
}

template<typename T>
T getConditionFromJSON(json object, const std::string &key, const std::string &val = "n") {
    return object.at(key).at("useCondition").get<bool>() ? object.at(key).at(val).get<T>() : 0;
}

StoppingCondition::StoppingCondition(json stopCon) :
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
    bool time = !time_eval || END_TIME > system_clock::now();
    bool accuracy = updateAccuracy(newBestVal) || (ACCURACY == 0 && NR_ACCURACY_ITERATIONS == 0);
    return eval && rects && time && accuracy;
}

void StoppingCondition::setStartNow() {
    if (time_eval) {
        END_TIME = time_point_cast<seconds>(system_clock::now() + minutes(mins));
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
