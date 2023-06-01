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
