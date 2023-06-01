#ifndef SIMOPTICON_CONTINUOUSPARAMETER_H
#define SIMOPTICON_CONTINUOUSPARAMETER_H


#include "Parameter.h"

class ContinuousParameter : public Parameter {
private:
    coordinate val;

public:
    ContinuousParameter(shared_ptr<ParameterDefinition> def, coordinate value);

    explicit ContinuousParameter(shared_ptr<ParameterDefinition> def);

    [[nodiscard]] coordinate getVal() const override;

    void setVal(coordinate val) override;
};


#endif //SIMOPTICON_CONTINUOUSPARAMETER_H
