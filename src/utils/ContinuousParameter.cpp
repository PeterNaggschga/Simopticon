#include "ContinuousParameter.h"

#include <stdexcept>

ContinuousParameter::ContinuousParameter(double min, double max, double val, const string &unit): Parameter(min, max, unit) {
    if (val < min || max < val) {
        throw invalid_argument("Value out of bounds");
    }
    ContinuousParameter::val = val;
}

ContinuousParameter::ContinuousParameter(double min, double max, const string &unit) : ContinuousParameter(min, max, (min + max) / 2, unit) {}

double ContinuousParameter::getVal() const {
    return val;
}

void ContinuousParameter::setVal(double val) {
    if (val < getMin() || getMax() < val) {
        throw invalid_argument("Value out of bounds");
    }
    ContinuousParameter::val = val;
}
