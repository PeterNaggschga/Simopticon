/**
 * @file
 * In this file, the implementation of the ParameterNormalizer class is defined.
 * @author Per Natzschka
 */

#include "ParameterNormalizer.h"

#include <utility>

#include "../../parameters/ContinuousParameter.h"
#include "../../parameters/ParameterDefinition.h"

ParameterNormalizer::ParameterNormalizer(std::list<std::shared_ptr<ParameterDefinition>> parameters) : parameters(
        std::move(parameters)) {
}

std::vector<dirCoordinate> ParameterNormalizer::normalize(const parameterCombination &params) {
    std::vector<dirCoordinate> result(params.size());
    for (const auto &param: params) {
        result.push_back((param->getVal() - param->getMin()) / (param->getMax() - param->getMin()));
    }
    return result;
}

parameterCombination ParameterNormalizer::denormalize(std::vector<dirCoordinate> cords) {
    parameterCombination result;
    int i = 0;
    for (const std::shared_ptr<ParameterDefinition> &def: parameters) {
        std::shared_ptr<Parameter> newParam(
                new ContinuousParameter(def, (coordinate) cords[i] * (def->getMax() - def->getMin()) + def->getMin()));
        result.push_back(newParam);
        i++;
    }
    return result;
}


