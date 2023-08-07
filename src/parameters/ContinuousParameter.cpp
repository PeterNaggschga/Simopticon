/**
 * @file
 * In this file, the implementation of the ContinuousParameter class is defined.
 */

#include "ContinuousParameter.h"

#include <stdexcept>
#include <utility>

ContinuousParameter::ContinuousParameter(shared_ptr<ParameterDefinition> def, coordinate value) : Parameter(
        std::move(def)),
                                                                                                  val(value) {
    if (value < getMin() || getMax() < value) {
        throw invalid_argument("Value out of bounds!");
    }
}

ContinuousParameter::ContinuousParameter(shared_ptr<ParameterDefinition> def) : ContinuousParameter(std::move(def),
                                                                                                    (getMin() +
                                                                                                     getMax()) / 2) {
}

coordinate ContinuousParameter::getVal() const {
    return val;
}

void ContinuousParameter::setVal(coordinate val) {
    if (val < getMin() || getMax() < val) {
        throw invalid_argument("Value out of bounds");
    }
    ContinuousParameter::val = val;
}
