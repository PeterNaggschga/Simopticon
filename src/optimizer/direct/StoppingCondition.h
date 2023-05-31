#ifndef SIMOPTICON_STOPPINGCONDITION_H
#define SIMOPTICON_STOPPINGCONDITION_H


#include <chrono>

using namespace std::chrono;

class StoppingCondition {
private:
    const unsigned long L = 0;
    const unsigned long M = 0;
    time_point<system_clock, seconds> T;
    const int mins;
    bool time_eval = false;

public:
    explicit StoppingCondition(unsigned long L = 0, unsigned long M = 0, int T = 0);

    void setStartNow();

    bool evaluate(unsigned long l = 0, unsigned long m = 0);
};


#endif //SIMOPTICON_STOPPINGCONDITION_H
