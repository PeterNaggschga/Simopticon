#include "DirectOptimizer.h"

#include <utility>

DirectOptimizer::DirectOptimizer(Controller &ctrl, list<ParameterDefinition> params, dimension D,
                                 StoppingCondition con) : Optimizer(ctrl, std::move(params)), D(D), stopCon(con),
                                                          normalizer(ParameterNormalizer(params)) {
}

void DirectOptimizer::runOptimization() {
    int m = 1, l = 0;

    auto base = HyRect(D, position::BASE, nullptr);
    addEstimatedValue(base);
    addActiveRect(base);
    //TODO init

    while (stopCon.evaluate(l, m)) {
        list<HyRect *> optimalRects = optimalRectangles();
        for (HyRect *rect: optimalRects) {
            for (HyRect newRect: rect->divide()) {
                //newRect.setValue(0);
                addActiveRect(newRect);
            }
            parentRects.insert(*rect);
            removeActiveRect(*rect);
        }
    }
}

map<vector<coordinate>, functionValue> DirectOptimizer::getValues(const list<vector<coordinate>> &points) {
    map<vector<Parameter *>, vector<coordinate>> paramToCord;
    list<vector<Parameter *>> paramList(points.size());
    for (const vector<coordinate> &point: points) {
        vector<Parameter *> paramVec = normalizer.denormalize(point);
        paramList.push_back(paramVec);
        paramToCord.insert(make_pair(paramVec, point));
    }

    map<vector<Parameter *>, functionValue> values = requestValues(paramList);

    map<vector<coordinate>, functionValue> result;
    for (pair<vector<Parameter *>, functionValue> pair: values) {
        result.insert(make_pair(paramToCord[pair.first], pair.second));
    }
    return result;
}

functionValue DirectOptimizer::estimatedValue(map<vector<coordinate>, functionValue> samples) {
    // TODO
    return 0;
}

list<HyRect *> DirectOptimizer::optimalRectangles() {
    // TODO
    return {};
}

void DirectOptimizer::addEstimatedValue(HyRect &rect) {
    rect.setValue(estimatedValue(getValues(rect.getSamplingVertices())));
}

void DirectOptimizer::addActiveRect(HyRect rect) {
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
}
