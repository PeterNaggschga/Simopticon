/**
 * @file
 * In this file, the implementation of the ContinuousParameter class is defined.
 */

#include "ContinuousParameter.h"

#include <stdexcept>
#include <utility>

ContinuousParameter::ContinuousParameter(std::shared_ptr<ParameterDefinition> def, coordinate value) :
        Parameter(std::move(def)), val(value) {
    if (value < getMin() || getMax() < value) {
        throw std::invalid_argument("Value out of bounds!");
    }
}

ContinuousParameter::ContinuousParameter(std::shared_ptr<ParameterDefinition> def) :
        ContinuousParameter(std::move(def), (getMin() + getMax()) / 2) {
}

coordinate ContinuousParameter::getVal() const {
    return val;
}

void ContinuousParameter::setVal(coordinate newVal) {
    if (newVal < getMin() || getMax() < newVal) {
        throw std::invalid_argument("Value out of bounds");
    }
    val = newVal;
}
