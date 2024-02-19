/**
 * @file
 * In this file, the implementation of the DirectStoppingCondition class is defined.
 */

#include "DirectStoppingCondition.h"

DirectStoppingCondition::DirectStoppingCondition(size_t evaluations, size_t hyrects, unsigned int minutes,
                                                 functionValue accuracy,
                                                 unsigned int accuracyIterations)
        : StoppingCondition(evaluations, minutes, accuracy, accuracyIterations),
          NR_HYRECTS(hyrects) {
}

DirectStoppingCondition::DirectStoppingCondition(const nlohmann::json &stopCon) :
        StoppingCondition(stopCon),
        NR_HYRECTS(getConditionFromJSON<size_t>(stopCon, "hyrects")) {
}


bool DirectStoppingCondition::evaluate(size_t evaluations, size_t hyrects, functionValue newBestVal) {
    bool otherEvaluations = StoppingCondition::evaluate(evaluations, newBestVal);
    bool rects = NR_HYRECTS == 0 || hyrects < NR_HYRECTS;
    return rects && otherEvaluations;
}

