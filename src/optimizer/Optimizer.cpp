/**
 * @file
 * In this file, the implementation of the Optimizer class is defined.
 * @author Per Natzschka
 */

#include "Optimizer.h"

#include "../controller/Controller.h"

#include <utility>

std::map<parameterCombination, functionValue>
Optimizer::requestValues(const std::list<parameterCombination> &params) {
    return controller.requestValues(params);
}

Optimizer::Optimizer(Controller &ctrl, std::list<std::shared_ptr<ParameterDefinition>> params) : controller(ctrl),
                                                                                       parameters(std::move(params)) {
}

ValueMap &Optimizer::getValueMap() const {
    return controller.getValueMap();
}

std::string Optimizer::getName() {
    return Status::getName();
}

std::string Optimizer::getStatus() {
    return Status::getStatus();
}

std::string Optimizer::getStatusBar() {
    return Status::getStatusBar();
}
