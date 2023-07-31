#include "DiscreteParameter.h"

#include <cmath>
#include <stdexcept>
#include <utility>

DiscreteParameter::DiscreteParameter(shared_ptr<ParameterDefinition> def, double step, double value) : Parameter(
        std::move(def)), step(step), times(floor(value / step)), offset(value - times * step) {
    if (value < getMin() || getMax() < value) {
        throw invalid_argument("Value out of bounds!");
    }
    if (getMax() - getMin() < step + offset) {
        throw invalid_argument("Step greater than bounds!");
    }
}

DiscreteParameter::DiscreteParameter(shared_ptr<ParameterDefinition> def, double step) : DiscreteParameter(
        std::move(def), step, fmod(getMax() + getMin() / 2, step)) {
}

int DiscreteParameter::getTimes() const {
    return times;
}

void DiscreteParameter::setTimes(int newTimes) {
    double val = newTimes * step + offset;
    if (getMax() < val || val < getMin()) {
        throw invalid_argument("Value out of bounds");
    }
    times = newTimes;
}

double DiscreteParameter::getStep() const {
    return step;
}

double DiscreteParameter::getOffset() const {
    return offset;
}

coordinate DiscreteParameter::getVal() const {
    return times * step + offset;
}

void DiscreteParameter::setVal(coordinate val) {
    setTimes((int) round((val - offset) / step));
}
