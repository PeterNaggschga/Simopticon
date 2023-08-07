#ifndef SIMOPTICON_PARAMETER_H
#define SIMOPTICON_PARAMETER_H

/**
 * @dir
 * In this directory, classes of the parameter module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the Parameter class is defined.
 */

#include "../Types.h"

#include <string>
#include <memory>

class ParameterDefinition;

using namespace std;

/**
 * This module defines framework-wide representations of the optimized parameters.
 * @defgroup parameters parameters
 */

/**
 * A class acting as the container of the value of a parameter defined by a ParameterDefinition.
 * @ingroup parameters
 */
class Parameter {
private:
    /**
     * Reference to the defining ParameterDefinition.
     */
    shared_ptr<ParameterDefinition> definition;

public:
    /**
     * Creates a Parameter with the given ParameterDefinition.
     * @param def: Definition of properties of the Parameter.
     */
    explicit Parameter(shared_ptr<ParameterDefinition> def);

    virtual ~Parameter() = default;

    /**
     * Returns the minimum value of the Parameter stored in ParameterDefinition#min of #definition.
     * @return A coordinate representing the minimum value.
     */
    [[nodiscard]] coordinate getMin() const;

    /**
     * Returns the maximum value of the Parameter stored in ParameterDefinition#max of #definition.
     * @return A coordinate representing the maximum value.
     */
    [[nodiscard]] coordinate getMax() const;

    /**
     * Returns the unit string of the Parameter stored in ParameterDefinition#unit of #definition.
     * @return A string reference containing the unit.
     */
    [[nodiscard]] const string &getUnit() const;

    /**
     * Returns the configuration string of the Parameter stored in ParameterDefinition#config of #definition.
     * @return A string reference containing the configuration.
     */
    [[nodiscard]] const string &getConfig() const;

    /**
     * Returns the current value of the Parameter.
     * @return A coordinate representing the value of the Parameter.
     */
    [[nodiscard]] virtual coordinate getVal() const = 0;

    /**
     * Sets the value of the Parameter to the given value.
     * @param val: Value to set the Parameter to.
     */
    virtual void setVal(coordinate val) = 0;

    /**
     * Checks if the current and the given Parameter objects are equal by comparing their value and #definition.
     * @param rhs: Parameter to be compared.
     * @return A boolean defining if the Parameter objects contain the same value for the same #definition.
     */
    bool operator==(const Parameter &rhs) const;

    /**
     * Checks if the current and the given Parameter objects are unequal by comparing their value and #definition.
     * Basically negates #operator==.
     * @param rhs: Parameter to be compared.
     * @return A boolean defining if the Parameter objects contain another value or another #definition.
     */
    bool operator!=(const Parameter &rhs) const;

    /**
     * Compares the value of the given Parameter objects.
     * @param rhs: Parameter to be compared.
     * @return A boolean defining if the value of this Parameter is less than that of the given Parameter.
     */
    bool operator<(const Parameter &rhs) const;

    /**
     * Compares the value of the given Parameter objects.
     * Basically calls #operator< on the switched inputs.
     * @param rhs: Parameter to be compared.
     * @return A boolean defining if the value of this Parameter is greater than that of the given Parameter.
     */
    bool operator>(const Parameter &rhs) const;

    /**
     * Compares the value of the given Parameter objects.
     * Basically negates #operator<.
     * @param rhs: Parameter to be compared.
     * @return A boolean defining if the value of this Parameter is less than or equal to that of the given Parameter.
     */
    bool operator<=(const Parameter &rhs) const;

    /**
     * Compares the value of the given Parameter objects.
     * Basically negates #operator>.
     * @param rhs: Parameter to be compared.
     * @return A boolean defining if the value of this Parameter is greater than or equal to that of the given Parameter.
     */
    bool operator>=(const Parameter &rhs) const;
};

#endif //SIMOPTICON_PARAMETER_H
