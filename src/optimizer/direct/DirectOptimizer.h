#ifndef SIMOPTICON_DIRECTOPTIMIZER_H
#define SIMOPTICON_DIRECTOPTIMIZER_H


#include "DirectTypes.h"
#include "DirectComparisonFunctions.h"
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
    size_t iterations = 0;
    StoppingCondition stopCon;
    Levels level;
    ParameterNormalizer normalizer;
    bool trackProgress;
    bool printValues;

    map<depth, set<shared_ptr<HyRect>, CmpSharedHyrect>, greater<>> activeRects;

    map<vector<dirCoordinate>, functionValue> getValues(const list<vector<dirCoordinate>> &points);

    static functionValue estimatedValue(const shared_ptr<HyRect> &rect, double k);

    list<shared_ptr<HyRect>> optimalRectangles(size_t nrRects, functionValue phi);

    void addActiveRects(const list<shared_ptr<HyRect>> &rects);

    void removeActiveRects(const list<shared_ptr<HyRect>> &rects);

    void saveProgress(functionValue bestVal, size_t evaluations, size_t nrRects) const;

    void saveValues();

public:
    DirectOptimizer(Controller &ctrl, const list<shared_ptr<ParameterDefinition>> &params, StoppingCondition con,
                    bool trackProgress, bool printValues);

    void runOptimization() override;

    string getName() override;

    string getStatus() override;

    string getStatusBar() override;

    size_t getPartitionSize();
};


#endif //SIMOPTICON_DIRECTOPTIMIZER_H
