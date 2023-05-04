#ifndef SIMOPTICON_PARAMETER_H
#define SIMOPTICON_PARAMETER_H

#include "ParameterDefinition.h"

class Parameter {
private:
    ParameterDefinition &definition;

public:
    explicit Parameter(ParameterDefinition &def);

    double getMin() const;

    double getMax() const;

    const string &getUnit() const;

    const string &getConfig() const;

    virtual double getVal() const = 0;

    virtual void setVal(double val) = 0;
};

#endif //SIMOPTICON_PARAMETER_H
