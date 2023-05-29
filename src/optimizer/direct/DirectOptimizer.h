#ifndef SIMOPTICON_DIRECTOPTIMIZER_H
#define SIMOPTICON_DIRECTOPTIMIZER_H


#include "../Optimizer.h"
#include "StoppingCondition.h"
#include "HyRect.h"

#include <set>
#include <unordered_set>

class StoppingCondition;

using namespace std;

class DirectOptimizer : public Optimizer {

private:
    const unsigned char D;
    StoppingCondition stopCon;
    set<HyRect> activeRects;
    unordered_set<HyRect> parentRects;

public:
    DirectOptimizer(Controller &ctrl, list<ParameterDefinition> params, unsigned char D, StoppingCondition con);

    void runOptimization() override;
};


#endif //SIMOPTICON_DIRECTOPTIMIZER_H
