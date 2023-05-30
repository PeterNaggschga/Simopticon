#include "Optimizer.h"

#include "../Controller.h"
#include "../parameters/Parameter.h"

#include <utility>

map<vector<Parameter *>, functionValue> Optimizer::requestValues(const list<vector<Parameter *>> &params) {
    return controller.requestValues(params);
}

Optimizer::Optimizer(Controller &ctrl, list<ParameterDefinition> params) : controller(ctrl),
                                                                           parameters(std::move(params)) {
}
