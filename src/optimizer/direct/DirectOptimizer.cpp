#include "DirectOptimizer.h"

#include <utility>

DirectOptimizer::DirectOptimizer(Controller &ctrl, list<ParameterDefinition> params, unsigned char D,
                                 StoppingCondition con) : Optimizer(ctrl, std::move(params)), D(D), stopCon(con) {
}

void DirectOptimizer::runOptimization() {
    int m = 1, l = 0;
    activeRects.insert(HyRect(D, position::BASE, nullptr));
    //TODO init

    while (stopCon.evaluate(l, m)) {

    }
}
