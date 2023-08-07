/**
 * @file
 * In this file, the implementation of the ParameterDefinition class is defined.
 */

#include "ParameterDefinition.h"

#include <utility>
#include <stdexcept>

ParameterDefinition::ParameterDefinition(coordinate min, coordinate max, string config, string unit) : min(min),
                                                                                                       max(max),
                                                                                                       config(std::move(
                                                                                                               config)),
                                                                                                       unit(std::move(
                                                                                                               unit)) {
    if (max < min) {
        throw invalid_argument("Maximum value must be greater than minimum!");
    }
}

coordinate ParameterDefinition::getMin() const {
    return min;
}

coordinate ParameterDefinition::getMax() const {
    return max;
}

const string &ParameterDefinition::getUnit() const {
    return unit;
}

const string &ParameterDefinition::getConfig() const {
    return config;
}
