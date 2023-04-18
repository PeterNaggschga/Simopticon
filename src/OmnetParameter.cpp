//
// Created by petern on 18.04.23.
//

#include "OmnetParameter.h"

#include <utility>

OMNeTParameter::OMNeTParameter(double min, double max, double val, string definition, string unit): Parameter(min, max, val, std::move(unit)) {
    this->definition = std::move(definition);
}

OMNeTParameter::OMNeTParameter(double min, double max, string definition, string unit): Parameter(min, max,std::move(unit)) {
    this->definition = std::move(definition);
}

const string &OMNeTParameter::getDefinition() const {
    return definition;
}

void OMNeTParameter::setDefinition(const string &definition) {
    OMNeTParameter::definition = definition;
}
