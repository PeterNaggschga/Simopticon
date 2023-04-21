#include "Parameter.h"

#include <stdexcept>

Parameter::Parameter(double min, double max, const string& unit) {
    if (max < min) {
        throw invalid_argument("Invalid parameter boundaries");
    }
    Parameter::min = min;
    setMax(max);
    setUnit(unit);
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

const string &Parameter::getUnit() const {
    return unit;
}

void Parameter::setUnit(const string &unit) {
    Parameter::unit = unit;
}
