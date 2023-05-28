#ifndef SIMOPTICON_STOPPINGCONDITION_H
#define SIMOPTICON_STOPPINGCONDITION_H


#include <chrono>

using namespace std::chrono;

class StoppingCondition {
private:
    const int L = 0;
    const int M = 0;
    const time_point<system_clock, seconds> T;
    bool time_eval = false;

public:
    explicit StoppingCondition(int L = 0, int M = 0, int T = 0);

    bool evaluate(int l = 0, int m = 0);
};


#endif //SIMOPTICON_STOPPINGCONDITION_H
