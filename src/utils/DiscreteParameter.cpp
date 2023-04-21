#include "DiscreteParameter.h"

#include <cmath>
#include <stdexcept>

DiscreteParameter::DiscreteParameter(double min, double max, int times, double step, double offset,
                                     const string &unit): Parameter(min, max, unit) {
    if (step > max - min) {
        throw invalid_argument("Step too large to fit bounds");
    }
    DiscreteParameter::step = step;
    setTimes(times);
    setOffset(offset);
}

DiscreteParameter::DiscreteParameter(double min, double max, double step, const string &unit): DiscreteParameter(min, max, (max + min) / 2, step, unit) {}


DiscreteParameter::DiscreteParameter(double min, double max, double val, double step, const string &unit): Parameter(min, max, unit) {
    if (step > max - min) {
        throw invalid_argument("Step too large to fit bounds");
    }
    DiscreteParameter::step = step;
    double timesDouble = val / step;
    setTimes(floor(timesDouble));
    setOffset(timesDouble - times);
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

void DiscreteParameter::setOffset(double offset) {
    double val = times * step + offset;
    if (getMax() < val || val < getMin()) {
        throw invalid_argument("Value out of bounds");
    }
    DiscreteParameter::offset = offset;
}

double DiscreteParameter::getVal() const {
    return times * step + offset;
}

void DiscreteParameter::setVal(double val) {
    times = (int) round((val - offset) / step);
}
