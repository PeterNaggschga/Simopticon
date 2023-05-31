#include "DirectOptimizer.h"
#include "../../utils/GrahamScan.h"

#include <utility>
#include <memory>

DirectOptimizer::DirectOptimizer(Controller &ctrl, list<ParameterDefinition> params, dimension D, StoppingCondition con,
                                 ValueMap &map) : Optimizer(ctrl, std::move(params), map), D(D), stopCon(con),
                                                  normalizer(ParameterNormalizer(params)) {
}

void DirectOptimizer::runOptimization() {
    stopCon.setStartNow();
    unsigned long m = 1, l = 0;
    auto base = HyRect(D, position::BASE, nullptr);
    addActiveRect(base);
    //TODO init

    while (stopCon.evaluate(l, m)) {
        for (HyRect rect: optimalRectangles(m)) {
            for (HyRect newRect: rect.divide()) {
                //TODO: alle gleichzeitig adden
                addActiveRect(newRect);
            }
            removeActiveRect(rect);
            m += 2;
        }
        l++;
    }
}

map<vector<dirCoordinate>, functionValue> DirectOptimizer::getValues(const list<vector<dirCoordinate>> &points) {
    map<vector<shared_ptr<Parameter>>, vector<dirCoordinate>> paramToCord;
    list<vector<shared_ptr<Parameter>>> paramList(points.size());
    for (const vector<dirCoordinate> &point: points) {
        vector<shared_ptr<Parameter>> paramVec = normalizer.denormalize(point);
        paramList.push_back(paramVec);
        paramToCord.insert(make_pair(paramVec, point));
    }

    map<vector<shared_ptr<Parameter>>, functionValue> values = requestValues(paramList);

    map<vector<dirCoordinate>, functionValue> result;
    for (pair<vector<shared_ptr<Parameter>>, functionValue> pair: values) {
        result.insert(make_pair(paramToCord[pair.first], pair.second));
    }
    return result;
}

functionValue DirectOptimizer::estimatedValue(map<vector<dirCoordinate>, functionValue> samples) {
    // TODO
    return 0;
}

list<HyRect> DirectOptimizer::optimalRectangles(unsigned long m) {
    list<HyRect> optimalPoints;
    unsigned long size = level.getRectSubsetSize(m);
    for (pair<depth, set<HyRect>> entry: activeRects) {
        size -= entry.second.size();
        HyRect rect = *entry.second.begin();
        optimalPoints.emplace_back(rect);
        if (size <= 0) {
            break;
        }
    }

    list<pair<HyRect, double>> kValues = GrahamScan::scan(optimalPoints);

    //TODO filtern nach epsilon

    return optimalPoints;
}

void DirectOptimizer::addActiveRect(HyRect rect) {
    functionValue value = estimatedValue(getValues(rect.getSamplingVertices()));
    rect.setAvgValue(value);
    depth depth = rect.getDepth();
    auto it = activeRects.find(depth);
    if (it != activeRects.end()) {
        it->second.insert(rect);
    } else {
        set<HyRect> newSet = {rect};
        activeRects.insert(make_pair(depth, newSet));
    }
}

void DirectOptimizer::removeActiveRect(HyRect rect) {
    depth depth = rect.getDepth();
    set<HyRect> set = activeRects[depth];
    set.erase(rect);
    if (set.empty()) {
        activeRects.erase(depth);
    }
    parentRects.insert(rect);
}
