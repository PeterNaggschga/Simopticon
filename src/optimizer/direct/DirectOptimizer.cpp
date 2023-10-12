/**
 * @file
 * In this file, the implementation of the DirectOptimizer class is defined.
 */

#include "DirectOptimizer.h"

#include "GrahamScan.h"
#include "hyrect/BaseRect.h"
#include "../../controller/ValueMap.h"

#include <utility>
#include <memory>
#include <fstream>

DirectOptimizer::DirectOptimizer(Controller &ctrl, const std::list<std::shared_ptr<ParameterDefinition>> &params,
                                 StoppingCondition con, bool trackProgress) : Optimizer(ctrl, params),
                                                                              D(params.size()), stopCon(con),
                                                                              normalizer(ParameterNormalizer(params)),
                                                                              level(Levels()),
                                                                              trackProgress(trackProgress) {
}

void DirectOptimizer::runOptimization() {
    stopCon.setStartNow();
    std::shared_ptr<HyRect> base(new BaseRect(D));
    addActiveRects({base});
    size_t evaluations = 2;
    size_t numberOfRectangles = 1;
    functionValue bestValue = getValueMap().getTopVals().front().second;
    if (trackProgress) {
        saveProgress(bestValue, evaluations, numberOfRectangles);
    }

    while (!aborted && stopCon.evaluate(evaluations, numberOfRectangles, bestValue)) {
        std::list<std::shared_ptr<HyRect>> newRects, oldRects;
        for (const std::shared_ptr<HyRect> &rect: optimalRectangles(numberOfRectangles, bestValue)) {
            for (const std::shared_ptr<HyRect> &newRect: HyRect::divide(rect)) {
                newRects.push_back(newRect);
            }
            oldRects.push_back(rect);
            numberOfRectangles += 2;
        }
        addActiveRects(newRects);
        removeActiveRects(oldRects);
        static functionValue oldPhi = bestValue;
        static unsigned char phaseIters = 0;
        bestValue = getValueMap().getTopVals().front().second;
        phaseIters++;
        level.nextLevel();
        if (level.isGlobal()) {
            if (bestValue < oldPhi - Levels::L3_EPSILON * std::abs(oldPhi - getValueMap().getMedian()) ||
                phaseIters > 35) {
                level.setGlobal(false);
                oldPhi = bestValue;
                phaseIters = 0;
            }
        }
        if (!level.isGlobal() && phaseIters > 3) {
            level.setGlobal(bestValue > oldPhi - Levels::L3_EPSILON * std::abs(oldPhi - getValueMap().getMedian()));
            oldPhi = bestValue;
            phaseIters = 0;
        }
        evaluations = getValueMap().getSize();
        iterations++;
        if (trackProgress) {
            saveProgress(bestValue, evaluations, numberOfRectangles);
        }
    }
}

std::map<std::vector<dirCoordinate>, functionValue>
DirectOptimizer::getValues(const std::list<std::vector<dirCoordinate>> &points) {
    std::map<parameterCombination, std::vector<dirCoordinate>> paramToCord;
    std::list<parameterCombination> paramList;
    for (const std::vector<dirCoordinate> &point: points) {
        parameterCombination paramVec = normalizer.denormalize(point);
        paramList.push_back(paramVec);
        paramToCord.insert(make_pair(paramVec, point));
    }

    std::map<parameterCombination, functionValue> values = requestValues(paramList);

    std::map<std::vector<dirCoordinate>, functionValue> result;
    for (std::pair<parameterCombination, functionValue> pair: values) {
        result.insert(make_pair(paramToCord[pair.first], pair.second));
    }
    return result;
}

functionValue DirectOptimizer::estimatedValue(const std::shared_ptr<HyRect> &rect, double k) {
    return rect->getAvgValue() - k * rect->getDiagonalLength();
}

std::list<std::shared_ptr<HyRect>> DirectOptimizer::optimalRectangles(size_t nrRects, functionValue phi) {
    std::list<std::shared_ptr<HyRect>> optimalPoints = level.getRectSubset(activeRects, nrRects);

    std::list<std::pair<std::shared_ptr<HyRect>, double>> kValues = GrahamScan::scan(optimalPoints);

    optimalPoints.clear();
    for (const auto &entry: kValues) {
        if (estimatedValue(entry.first, entry.second) <=
            phi - level.getEpsilon() * std::abs(phi - getValueMap().getMedian())) {
            optimalPoints.push_back(entry.first);
        }
    }

    return optimalPoints;
}

void DirectOptimizer::addActiveRects(const std::list<std::shared_ptr<HyRect>> &rects) {
    struct hashFunction {
        size_t operator()(const std::vector<dirCoordinate> &v) const {
            std::hash<dirCoordinate> hash;
            size_t res = 0;
            for (dirCoordinate cord: v) {
                res ^= hash(cord);
            }
            return hash(res);
        }
    };

    std::map<std::shared_ptr<HyRect>, std::array<std::vector<dirCoordinate>, 2>> rectToVec;
    std::unordered_set<std::vector<dirCoordinate>, hashFunction> vecs;
    for (const auto &rect: rects) {
        auto vertices = rect->getSamplingVertices();
        rectToVec.insert(make_pair(rect, vertices));
        vecs.insert(vertices.begin(), vertices.end());
    }

    auto samples = getValues(std::list<std::vector<dirCoordinate>>(vecs.begin(), vecs.end()));

    for (const auto &entry: rectToVec) {
        entry.first->setAvgValue((samples[entry.second[0]] + samples[entry.second[1]]) / 2);
        depth depth = entry.first->getDepth();
        auto it = activeRects.find(depth);
        if (it != activeRects.end()) {
            it->second.insert(entry.first);
        } else {
            std::set<std::shared_ptr<HyRect>, CmpSharedHyrect> newSet = {entry.first};
            activeRects.insert(make_pair(depth, newSet));
        }
    }
}

void DirectOptimizer::removeActiveRects(const std::list<std::shared_ptr<HyRect>> &rects) {
    for (const auto &rect: rects) {
        depth depth = rect->getDepth();
        std::set<std::shared_ptr<HyRect>, CmpSharedHyrect> &set = activeRects[depth];
        set.erase(rect);
        if (set.empty()) {
            activeRects.erase(depth);
        }
    }
}

void DirectOptimizer::saveProgress(functionValue bestVal, size_t evaluations, size_t nrRects) const {
    static std::ofstream out;
    if (!out.is_open()) {
        if (!std::filesystem::exists("results/progress.csv")) {
            std::filesystem::create_directories("results");
        }
        out.open("results/progress.csv", std::ios::out | std::ios::trunc);
        out << "Iteration;" << "Evaluations;" << "Rectangles;" << "Minimum\n";
    }
    out << iterations << ";" << evaluations << ";" << nrRects << ";" << bestVal << std::endl;
}

std::string DirectOptimizer::getName() {
    return "Direct-Optimizer";
}

std::string DirectOptimizer::getStatus() {
    std::string status = "Rectangles:\t\t\t" + std::to_string(getPartitionSize()) + "\n";
    status += "Iterations:\t\t\t" + std::to_string(iterations) + "\n";
    status += "Iterations without improvement:\t" + std::to_string(stopCon.getIterationsSinceImprov()) + "\n";
    status += "Level:\t\t\t\t" + std::to_string(level.getLevel());

    return status;
}

std::string DirectOptimizer::getStatusBar() {
    return "Optimizer running...";
}

size_t DirectOptimizer::getPartitionSize() {
    size_t size = 0;
    for (const auto &entry: activeRects) {
        size += entry.second.size();
    }
    return size;
}
