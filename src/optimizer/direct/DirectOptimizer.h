#ifndef SIMOPTICON_DIRECTOPTIMIZER_H
#define SIMOPTICON_DIRECTOPTIMIZER_H


#include "../Optimizer.h"
#include "StoppingCondition.h"
#include "HyRect.h"
#include "../../Types.h"

#include <set>
#include <unordered_set>

class StoppingCondition;

using namespace std;

class DirectOptimizer : public Optimizer {

private:
    const dimension D;
    StoppingCondition stopCon;
    map<depth, set<HyRect>> activeRects;
    unordered_set<HyRect> parentRects;

    void addActiveRect(HyRect rect);

    void removeActiveRect(HyRect rect);

    list<HyRect *> optimalRectangles();

public:
    DirectOptimizer(Controller &ctrl, list<ParameterDefinition> params, dimension D, StoppingCondition con);

    void runOptimization() override;
};


#endif //SIMOPTICON_DIRECTOPTIMIZER_H
