#include "StoppingCondition.h"

StoppingCondition::StoppingCondition(int L, int M, int T) : L(L), M(M), T(time_point_cast<seconds>(
        system_clock::now() + minutes(T))) {
    if (T != 0) {
        time_eval = true;
    }
}

bool StoppingCondition::evaluate(int l, int m) {
    return (L == 0 || l < L) && (M == 0 || m < M) && (!time_eval || T > system_clock::now());
}
