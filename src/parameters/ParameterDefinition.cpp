#include "ParameterDefinition.h"

#include <utility>
#include <stdexcept>

ParameterDefinition::ParameterDefinition(double min, double max, string config, string unit):min(min), max(max), config(std::move(config)), unit(std::move(unit)) {
    if (max < min) {
        throw invalid_argument("Maximum value must be greater than minimum!");
    }
}

double ParameterDefinition::getMin() const {
    return min;
}

double ParameterDefinition::getMax() const {
    return max;
}

const string &ParameterDefinition::getUnit() const {
    return unit;
}

const string &ParameterDefinition::getConfig() const {
    return config;
}
