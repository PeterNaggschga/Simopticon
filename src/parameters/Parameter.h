#ifndef SIMOPTICON_PARAMETER_H
#define SIMOPTICON_PARAMETER_H

#include "ParameterDefinition.h"

class Parameter {
private:
    ParameterDefinition &definition;

public:
    Parameter(ParameterDefinition def);

    double getMin() const;

    double getMax() const;

    virtual double getVal() const = 0;

    virtual void setVal(double val) = 0;

    const string &getUnit() const;

    const string &getConfig() const;
};

#endif //SIMOPTICON_PARAMETER_H
