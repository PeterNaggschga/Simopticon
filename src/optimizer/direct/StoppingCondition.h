#ifndef SIMOPTICON_STOPPINGCONDITION_H
#define SIMOPTICON_STOPPINGCONDITION_H


#include "../../Types.h"
#include "nlohmann/json.hpp"

#include <chrono>
#include <cmath>

using namespace std::chrono;
using json = nlohmann::json;

class StoppingCondition {
private:
    const size_t NR_EVALUATIONS;
    const size_t NR_HYRECTS;

    time_point<system_clock, seconds> END_TIME;
    const unsigned int mins;
    bool time_eval;

    const functionValue ACCURACY;
    const unsigned int NR_ACCURACY_EVALUATIONS;
    functionValue bestVal = INFINITY;
    unsigned int evaluationsSinceImprov = 0;

    bool updateAccuracy(functionValue newBestVal);

public:
    explicit StoppingCondition(size_t evaluations = 0, size_t hyrects = 0, unsigned int minutes = 0,
                               functionValue accuracy = 0, unsigned int accuracyIterations = 0);

    explicit StoppingCondition(json stopCon);

    void setStartNow();

    bool evaluate(size_t evaluations, size_t hyrects, functionValue newBestVal);
};


#endif //SIMOPTICON_STOPPINGCONDITION_H
