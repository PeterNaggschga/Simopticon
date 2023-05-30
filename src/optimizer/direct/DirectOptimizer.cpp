#include "DirectOptimizer.h"

#include <utility>

DirectOptimizer::DirectOptimizer(Controller &ctrl, list<ParameterDefinition> params, unsigned char D,
                                 StoppingCondition con) : Optimizer(ctrl, std::move(params)), D(D), stopCon(con) {
}

void DirectOptimizer::runOptimization() {
    int m = 1, l = 0;

    auto base = HyRect(D, position::BASE, nullptr);
    //base.setValue(0);
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

list<HyRect *> DirectOptimizer::optimalRectangles() {

    return {};
}

void DirectOptimizer::addActiveRect(HyRect rect) {
    auto depth = rect.getDepth();
    auto it = activeRects.find(depth);
    if (it != activeRects.end()) {
        it->second.insert(rect);
    } else {
        set<HyRect> newSet = {rect};
        activeRects.insert(make_pair(depth, newSet));
    }
}

void DirectOptimizer::removeActiveRect(HyRect rect) {
    auto depth = rect.getDepth();
    set<HyRect> set = activeRects[depth];
    set.erase(rect);
    if (set.empty()) {
        activeRects.erase(depth);
    }
}

