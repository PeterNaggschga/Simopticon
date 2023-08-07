/**
 * @file
 * In this file, the implementation of the ParameterNormalizer class is defined.
 */

#include "ParameterNormalizer.h"

#include <utility>

#include "../../parameters/ContinuousParameter.h"
#include "../../parameters/ParameterDefinition.h"

ParameterNormalizer::ParameterNormalizer(list<shared_ptr<ParameterDefinition>> parameters) : parameters(
        std::move(parameters)) {
}

vector<dirCoordinate> ParameterNormalizer::normalize(const vector<shared_ptr<Parameter>> &params) {
    vector<dirCoordinate> result(params.size());
    for (const auto &param: params) {
        result.push_back((param->getVal() - param->getMin()) / (param->getMax() - param->getMin()));
    }
    return result;
}

vector<shared_ptr<Parameter>> ParameterNormalizer::denormalize(vector<dirCoordinate> cords) {
    vector<shared_ptr<Parameter>> result;
    int i = 0;
    for (const shared_ptr<ParameterDefinition> &def: parameters) {
        shared_ptr<Parameter> newParam(
                new ContinuousParameter(def, (coordinate) cords[i] * (def->getMax() - def->getMin()) + def->getMin()));
        result.push_back(newParam);
        i++;
    }
    return result;
}


