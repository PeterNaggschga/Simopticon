#ifndef SIMOPTICON_CONTINUOUSPARAMETER_H
#define SIMOPTICON_CONTINUOUSPARAMETER_H

/**
 * @file
 * In this file, the header of the ContinuousParameter class is defined.
 */

#include "Parameter.h"

/**
 * Implements a Parameter using continuos values in the form of floating point numbers.
 * @ingroup parameters
 */
class ContinuousParameter : public Parameter {
private:
    /**
     * Value of the ContinuousParameter.
     */
    coordinate val;

public:
    /**
     * Creates a ContinuousParameter with the given ParameterDefinition and value.
     * * Checks if given value is in bounds set by the ParameterDefinition.
     * @param def: ParameterDefinition of the Parameter.
     * @param value: Initial value of the Parameter.
     */
    ContinuousParameter(shared_ptr<ParameterDefinition> def, coordinate value);

    /**
     * Creates a ContinuousParameter with the given ParameterDefinition and the initial value being the mean between minimum and maximum.
     * @param def: ParameterDefinition of the Parameter.
     */
    explicit ContinuousParameter(shared_ptr<ParameterDefinition> def);

    /**
     * Returns the current value of #val.
     * @return A coordinate representing the value of the ContinuousParameter.
     */
    [[nodiscard]] coordinate getVal() const override;

    /**
     * Sets the value of #val to the given value.
     * Checks if given value is in bounds set by the ParameterDefinition.
     * @param val: Value to set the ContinuousParameter to.
     */
    void setVal(coordinate val) override;
};


#endif //SIMOPTICON_CONTINUOUSPARAMETER_H
