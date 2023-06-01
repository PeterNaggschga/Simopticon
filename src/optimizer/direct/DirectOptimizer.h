#ifndef SIMOPTICON_DIRECTOPTIMIZER_H
#define SIMOPTICON_DIRECTOPTIMIZER_H


#include "DirectTypes.h"
#include "../Optimizer.h"
#include "StoppingCondition.h"
#include "hyrect/HyRect.h"
#include "ParameterNormalizer.h"
#include "Levels.h"

#include <set>
#include <unordered_set>
#include <memory>

class StoppingCondition;

using namespace std;

class DirectOptimizer : public Optimizer {

private:
    const dimension D;
    StoppingCondition stopCon;
    Levels level = Levels();
    ParameterNormalizer normalizer;
    map<depth, set<shared_ptr<HyRect>>> activeRects;

    map<vector<dirCoordinate>, functionValue> getValues(const list<vector<dirCoordinate>> &points);

    functionValue estimatedValue(map<vector<dirCoordinate>, functionValue> samples);

    list<shared_ptr<HyRect>> optimalRectangles(unsigned long m);

    void addActiveRects(const list<shared_ptr<HyRect>> &rects);

    void removeActiveRect(const shared_ptr<HyRect> &rect);

public:
    DirectOptimizer(Controller &ctrl, list<shared_ptr<ParameterDefinition>> params, dimension D, StoppingCondition con,
                    ValueMap &map);

    void runOptimization() override;
};


#endif //SIMOPTICON_DIRECTOPTIMIZER_H
