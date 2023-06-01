#include "DirectOptimizer.h"

#include "../../utils/GrahamScan.h"
#include "hyrect/BaseRect.h"
#include "../../controller/ValueMap.h"

#include <utility>
#include <memory>

DirectOptimizer::DirectOptimizer(Controller &ctrl, list<shared_ptr<ParameterDefinition>> params, dimension D,
                                 StoppingCondition con) : Optimizer(ctrl, std::move(params)), D(D), stopCon(con),
                                                          normalizer(ParameterNormalizer(params)) {
}

void DirectOptimizer::runOptimization() {
    stopCon.setStartNow();
    shared_ptr<HyRect> base(new BaseRect(D));
    addActiveRects({base});
    unsigned long m = 1, l = 2;
    functionValue phi;

    while (stopCon.evaluate(l, m)) {
        phi = getValueMap().getTopVals().front().second;
        list<shared_ptr<HyRect>> newRects;
        for (const shared_ptr<HyRect> &rect: optimalRectangles(m, phi)) {
            for (const shared_ptr<HyRect> &newRect: rect->divide(rect)) {
                newRects.push_back(newRect);
            }
            removeActiveRect(rect);
            m += 2;
        }
        addActiveRects(newRects);
        l = getValueMap().getSize();
        level.nextLevel();
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

functionValue DirectOptimizer::estimatedValue(const shared_ptr<HyRect> &rect, double k) {
    return rect->getAvgValue() - k * rect->getDiagonalLength() / 2;
}

list<shared_ptr<HyRect>> DirectOptimizer::optimalRectangles(unsigned long m, functionValue phi) {
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

    optimalPoints.clear();
    for (const auto &entry: kValues) {
        if (entry.second == INFINITY || estimatedValue(entry.first, entry.second) <=
                                        phi - level.getEpsilon() * abs(phi - getValueMap().getMedian())) {
            optimalPoints.push_back(entry.first);
        }
    }

    return optimalPoints;
}

void DirectOptimizer::addActiveRects(const list<shared_ptr<HyRect>> &rects) {
    struct hashFunction {
        size_t operator()(const vector<dirCoordinate> &v) const {
            hash<dirCoordinate> hash;
            size_t res = 0;
            for (dirCoordinate cord: v) {
                res ^= hash(cord);
            }
            return hash(res);
        }
    };

    map<shared_ptr<HyRect>, array<vector<dirCoordinate>, 2>> rectToVec;
    unordered_set<vector<dirCoordinate>, hashFunction> vecs;
    for (const auto &rect: rects) {
        auto vertices = rect->getSamplingVertices();
        rectToVec.insert(make_pair(rect, vertices));
        vecs.insert(vertices.begin(), vertices.end());
    }

    auto samples = getValues(list<vector<dirCoordinate>>(vecs.begin(), vecs.end()));

    for (const auto &entry: rectToVec) {
        entry.first->setAvgValue((samples[entry.second[0]] + samples[entry.second[1]]) / 2);
        depth depth = entry.first->getDepth();
        auto it = activeRects.find(depth);
        if (it != activeRects.end()) {
            it->second.insert(entry.first);
        } else {
            set<shared_ptr<HyRect>> newSet = {entry.first};
            activeRects.insert(make_pair(depth, newSet));
        }
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
