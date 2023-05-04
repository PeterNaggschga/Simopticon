#include "DiscreteParameter.h"

#include <cmath>
#include <stdexcept>

DiscreteParameter::DiscreteParameter(ParameterDefinition &def, double step, double value) : Parameter(def), step(step),
                                                                                            times(floor(value / step)),
                                                                                            offset(value -
                                                                                                   times * step) {
    if (value < getMin() || getMax() < value) {
        throw invalid_argument("Value out of bounds!");
    }
    if (getMax() - getMin() < step + offset) {
        throw invalid_argument("Step greater than bounds!");
    }
}

DiscreteParameter::DiscreteParameter(ParameterDefinition &def, double step) : DiscreteParameter(def, step,
                                                                                                fmod(getMax() +
                                                                                                     getMin() / 2,
                                                                                                     step)) {
}

int DiscreteParameter::getTimes() const {
    return times;
}

void DiscreteParameter::setTimes(int times) {
    double val = times * step + offset;
    if (getMax() < val || val < getMin()) {
        throw invalid_argument("Value out of bounds");
    }
    DiscreteParameter::times = times;
}

double DiscreteParameter::getStep() const {
    return step;
}

double DiscreteParameter::getOffset() const {
    return offset;
}

double DiscreteParameter::getVal() const {
    return times * step + offset;
}

void DiscreteParameter::setVal(double val) {
    setTimes((int) round((val - offset) / step));
}
