/**
 * @file
 * In this file the implementation of the Parameter class is defined.
 */

#include "Parameter.h"

#include "ParameterDefinition.h"

#include <utility>

Parameter::Parameter(shared_ptr<ParameterDefinition> def) : definition(std::move(def)) {

}

coordinate Parameter::getMin() const {
    return definition->getMin();
}

coordinate Parameter::getMax() const {
    return definition->getMax();
}

const string &Parameter::getUnit() const {
    return definition->getUnit();
}

const string &Parameter::getConfig() const {
    return definition->getConfig();
}

bool Parameter::operator==(const Parameter &rhs) const {
    return getVal() == rhs.getVal() && definition == rhs.definition;
}

bool Parameter::operator!=(const Parameter &rhs) const {
    return !(rhs == *this);
}

bool Parameter::operator<(const Parameter &rhs) const {
    return getVal() < rhs.getVal();
}

bool Parameter::operator>(const Parameter &rhs) const {
    return rhs < *this;
}

bool Parameter::operator<=(const Parameter &rhs) const {
    return !(rhs < *this);
}

bool Parameter::operator>=(const Parameter &rhs) const {
    return !(*this < rhs);
}
