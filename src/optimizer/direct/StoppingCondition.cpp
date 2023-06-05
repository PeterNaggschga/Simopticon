#include "StoppingCondition.h"

StoppingCondition::StoppingCondition(size_t evaluations, size_t hyrects, unsigned int minutes,
                                     functionValue accuracy,
                                     unsigned int accuracyIterations)
        : NR_EVALUATIONS(evaluations), NR_HYRECTS(hyrects), mins(minutes), time_eval(minutes != 0), ACCURACY(accuracy),
          NR_ACCURACY_EVALUATIONS(accuracyIterations) {
}

bool StoppingCondition::evaluate(size_t evaluations, size_t hyrects, functionValue newBestVal) {
    bool eval = NR_EVALUATIONS == 0 || evaluations < NR_EVALUATIONS;
    bool rects = NR_HYRECTS == 0 || hyrects < NR_HYRECTS;
    bool time = !time_eval || END_TIME > system_clock::now();
    bool accuracy = (ACCURACY == 0 && NR_EVALUATIONS == 0) || updateAccuracy(newBestVal);
    return eval && rects && time && accuracy;
}

void StoppingCondition::setStartNow() {
    if (time_eval) {
        END_TIME = time_point_cast<seconds>(system_clock::now() + minutes(mins));
    }
}

bool StoppingCondition::updateAccuracy(functionValue newBestVal) {
    if (newBestVal <= bestVal - ACCURACY) {
        evaluationsSinceImprov = 0;
        bestVal = newBestVal;
    } else {
        evaluationsSinceImprov++;
    }
    return evaluationsSinceImprov < NR_ACCURACY_EVALUATIONS;
}
