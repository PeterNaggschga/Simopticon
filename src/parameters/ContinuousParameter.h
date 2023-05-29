#ifndef SIMOPTICON_CONTINUOUSPARAMETER_H
#define SIMOPTICON_CONTINUOUSPARAMETER_H


#include "Parameter.h"

class ContinuousParameter : public Parameter {
private:
    double val;

public:
    ContinuousParameter(ParameterDefinition &def, double value);

    explicit ContinuousParameter(ParameterDefinition &def);

    [[nodiscard]] double getVal() const override;

    void setVal(double val) override;
};


#endif //SIMOPTICON_CONTINUOUSPARAMETER_H
