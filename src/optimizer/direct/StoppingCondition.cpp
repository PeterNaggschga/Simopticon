#include "StoppingCondition.h"

StoppingCondition::StoppingCondition(int L, int M, int T) : L(L), M(M), mins(T) {
    if (T != 0) {
        time_eval = true;
    }
}

bool StoppingCondition::evaluate(int l, int m) {
    return (L == 0 || l < L) && (M == 0 || m < M) && (!time_eval || T > system_clock::now());
}

void StoppingCondition::setStartNow() {
    if (time_eval) {
        T = time_point_cast<seconds>(system_clock::now() + minutes(mins));
    }
}
