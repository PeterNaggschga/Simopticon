#include "DirectOptimizer.h"
#include "../../utils/GrahamScan.h"
#include "hyrect/BaseRect.h"

#include <utility>
#include <memory>

DirectOptimizer::DirectOptimizer(Controller &ctrl, list<shared_ptr<ParameterDefinition>> params, dimension D,
                                 StoppingCondition con,
                                 ValueMap &map) : Optimizer(ctrl, std::move(params), map), D(D), stopCon(con),
                                                  normalizer(ParameterNormalizer(params)) {
}

void DirectOptimizer::runOptimization() {
    stopCon.setStartNow();
    unsigned long m = 1, l = 0;
    shared_ptr<HyRect> base(new BaseRect(D));
    addActiveRect(base);
    //TODO init

    while (stopCon.evaluate(l, m)) {
        for (const shared_ptr<HyRect> &rect: optimalRectangles(m)) {
            for (const shared_ptr<HyRect> &newRect: rect->divide(rect)) {
                //TODO: alle gleichzeitig adden
                addActiveRect(newRect);
            }
            removeActiveRect(rect);
            m += 2;
        }
        l++; // TODO valueMap abfragen
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

list<shared_ptr<HyRect>> DirectOptimizer::optimalRectangles(unsigned long m) {
    list<shared_ptr<HyRect>> optimalPoints;
    unsigned long size = level.getRectSubsetSize(m);
    for (pair<depth, set<shared_ptr<HyRect>>> entry: activeRects) {
        size -= entry.second.size();
        shared_ptr<HyRect> rect = *entry.second.begin();
        optimalPoints.emplace_back(rect);
        if (size <= 0) {
            break;
        }
    }

    list<pair<shared_ptr<HyRect>, double>> kValues = GrahamScan::scan(optimalPoints);

    //TODO filtern nach epsilon

    return optimalPoints;
}

void DirectOptimizer::addActiveRect(const shared_ptr<HyRect> &rect) {
    array<vector<dirCoordinate>, 2> vertices = rect->getSamplingVertices();
    auto samples = getValues(list<vector<dirCoordinate>>({vertices[0], vertices[1]}));
    rect->setAvgValue(estimatedValue(samples));
    depth depth = rect->getDepth();
    auto it = activeRects.find(depth);
    if (it != activeRects.end()) {
        it->second.insert(rect);
    } else {
        set<shared_ptr<HyRect>> newSet = {rect};
        activeRects.insert(make_pair(depth, newSet));
    }
}

void DirectOptimizer::removeActiveRect(const shared_ptr<HyRect> &rect) {
    depth depth = rect->getDepth();
    set<shared_ptr<HyRect>> &set = activeRects[depth];
    set.erase(rect);
    if (set.empty()) {
        activeRects.erase(depth);
    }
}
