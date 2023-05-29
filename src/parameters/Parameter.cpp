#include "Parameter.h"

#include "ParameterDefinition.h"

Parameter::Parameter(ParameterDefinition &def) : definition(def) {

}

double Parameter::getMin() const {
    return definition.getMin();
}

double Parameter::getMax() const {
    return definition.getMax();
}

const string &Parameter::getUnit() const {
    return definition.getUnit();
}

const string &Parameter::getConfig() const {
    return definition.getConfig();
}
