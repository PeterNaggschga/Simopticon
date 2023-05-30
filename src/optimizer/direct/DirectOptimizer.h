#ifndef SIMOPTICON_DIRECTOPTIMIZER_H
#define SIMOPTICON_DIRECTOPTIMIZER_H


#include "DirectTypes.h"
#include "../Optimizer.h"
#include "StoppingCondition.h"
#include "HyRect.h"
#include "ParameterNormalizer.h"

#include <set>
#include <unordered_set>

class StoppingCondition;

using namespace std;

class DirectOptimizer : public Optimizer {

private:
    const dimension D;
    StoppingCondition stopCon;
    ParameterNormalizer normalizer;
    map<depth, set<HyRect>> activeRects;
    unordered_set<HyRect> parentRects;

    map<vector<coordinate>, functionValue> getValues(const list<vector<coordinate>> &points);

    functionValue estimatedValue(map<vector<coordinate>, functionValue> samples);

    list<HyRect *> optimalRectangles();

    void addEstimatedValue(HyRect &rect);

    void addActiveRect(HyRect rect);

    void removeActiveRect(HyRect rect);

public:
    DirectOptimizer(Controller &ctrl, list<ParameterDefinition> params, dimension D, StoppingCondition con);

    void runOptimization() override;
};


#endif //SIMOPTICON_DIRECTOPTIMIZER_H
