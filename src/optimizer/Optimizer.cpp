#include "Optimizer.h"

#include "../Controller.h"
#include "../parameters/Parameter.h"

#include <utility>

map<vector<Parameter>, double> Optimizer::getValues(const list<vector<Parameter>> &params) {
    return controller.getValues(params);
}

Optimizer::Optimizer(Controller &ctrl, list<ParameterDefinition> params) : controller(ctrl),
                                                                           parameters(std::move(params)) {
}
