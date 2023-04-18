#include "Parameter.h"

Parameter::Parameter(double min, double max, double val, string unit) {
    if (max < min || val < min || max < val) {
        throw invalid_argument("Invalid parameter boundaries");
    }
    this->min = min;
    this->max = max;
    this->val = val;
    this->unit = std::move(unit);
}

Parameter::Parameter(double min, double max, string unit) {
    if (max < min) {
        throw invalid_argument("Invalid parameter boundaries");
    }
    this->min = min;
    this->max = max;
    this->val = (min + max) / 2;
    this->unit = std::move(unit);
}

double Parameter::getMin() const {
    return min;
}

void Parameter::setMin(double min) {
    if (max < min) {
        throw invalid_argument("Minimum smaller than maximum");
    }
    Parameter::min = min;
}

double Parameter::getMax() const {
    return max;
}

void Parameter::setMax(double max) {
    if (max < min) {
        throw invalid_argument("Maximum greater than minimum");
    }
    Parameter::max = max;
}

double Parameter::getVal() const {
    return val;
}

void Parameter::setVal(double val) {
    if (max < val || val < min) {
        throw invalid_argument("Value out of bounds");
    }
    Parameter::val = val;
}

const string &Parameter::getUnit() const {
    return unit;
}

void Parameter::setUnit(const string &unit) {
    Parameter::unit = unit;
}
