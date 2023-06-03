#ifndef SIMOPTICON_STOPPINGCONDITION_H
#define SIMOPTICON_STOPPINGCONDITION_H


#include "../../Types.h"

#include <chrono>
#include <cmath>

using namespace std::chrono;

class StoppingCondition {
private:
    const unsigned long NR_EVALUATIONS;
    const unsigned long NR_HYRECTS;

    time_point<system_clock, seconds> END_TIME;
    const unsigned int mins;
    bool time_eval;

    const functionValue ACCURACY;
    const unsigned int NR_ACCURACY_EVALUATIONS;
    functionValue bestVal = INFINITY;
    unsigned int evaluationsSinceImprov = 0;

    bool updateAccuracy(functionValue newBestVal);

public:
    explicit StoppingCondition(unsigned long evaluations = 0, unsigned long hyrects = 0, unsigned int minutes = 0,
                               functionValue accuracy = 0, unsigned int accuracyIterations = 0);

    void setStartNow();

    bool evaluate(unsigned long evaluations, unsigned long hyrects, functionValue newBestVal);
};


#endif //SIMOPTICON_STOPPINGCONDITION_H
