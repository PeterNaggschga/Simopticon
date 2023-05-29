#ifndef SIMOPTICON_DISCRETEPARAMETER_H
#define SIMOPTICON_DISCRETEPARAMETER_H


#include "Parameter.h"

class DiscreteParameter : public Parameter {
private:
    int times;
    double step;
    double offset = 0;


public:
    DiscreteParameter(ParameterDefinition &def, double step, double value);

    DiscreteParameter(ParameterDefinition &def, double step);

    [[nodiscard]] int getTimes() const;

    void setTimes(int times);

    [[nodiscard]] double getStep() const;

    [[nodiscard]] double getOffset() const;

    [[nodiscard]] double getVal() const override;

    void setVal(double val) override;
};


#endif //SIMOPTICON_DISCRETEPARAMETER_H
