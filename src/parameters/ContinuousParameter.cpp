#include "ContinuousParameter.h"

#include <stdexcept>

ContinuousParameter::ContinuousParameter(ParameterDefinition &def, double value): Parameter(def), val(value) {
    if (value < getMin() || getMax() < value) {
        throw invalid_argument("Value out of bounds!");
    }
}

ContinuousParameter::ContinuousParameter(ParameterDefinition &def): ContinuousParameter(def, (getMin() + getMax()) / 2) {
}

double ContinuousParameter::getVal() const {
    return val;
}

void ContinuousParameter::setVal(double val) {
    if (val < getMin() || getMax() < val) {
        throw invalid_argument("Value out of bounds");
    }
    ContinuousParameter::val = val;
}
