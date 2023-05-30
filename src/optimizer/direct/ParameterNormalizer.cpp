#include "ParameterNormalizer.h"

#include "../../parameters/ContinuousParameter.h"
#include "../../parameters/ParameterDefinition.h"

ParameterNormalizer::ParameterNormalizer(list<ParameterDefinition> &parameters) : parameters(parameters) {
}

vector<coordinate> ParameterNormalizer::normalize(const vector<Parameter *> &params) {
    vector<coordinate> result(params.size());
    for (auto &param: params) {
        result.push_back((param->getVal() - param->getMin()) / (param->getMax() - param->getMin()));
        delete param;
    }
    return result;
}

vector<Parameter *> ParameterNormalizer::denormalize(vector<coordinate> cords) {
    vector<Parameter *> result;
    int i = 0;
    for (ParameterDefinition def: parameters) {
        result.push_back(new ContinuousParameter(def, cords[i] * (def.getMax() - def.getMin()) + def.getMin()));
        i++;
    }
    return result;
}


