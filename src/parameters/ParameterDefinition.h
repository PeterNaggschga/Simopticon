#ifndef SIMOPTICON_PARAMETERDEFINITION_H
#define SIMOPTICON_PARAMETERDEFINITION_H


#include "../Types.h"

#include <string>

using namespace std;

/**
 * A class storing information on the properties of parameters that are being optimized.
 */
class ParameterDefinition {
private:
    /**
     * Minimum value of the Parameter.
     */
    const coordinate min;
    /**
     * Maximum value of the Parameter.
     */
    const coordinate max;
    /**
     * Unit of the Parameter (optional).
     */
    const string unit;
    /**
     * String containing configuration details of the Parameter (optional).
     * May be used to transfer configuration information for SimulationRunner.
     */
    const string config;

public:
    /**
     * Creates a ParameterDefinition with the given minimum, maximum, configuration string and unit.
     * @param min: Minimum value of the Parameter.
     * @param max: Maximum value of the Parameter.
     * @param config: Configuration string for the Parameter (optional).
     * @param unit: Unit of the Parameter (optional)
     */
    ParameterDefinition(coordinate min, coordinate max, string config = "", string unit = "");

    /**
     * Returns the minimum value of the Parameter stored in ParameterDefinition::min.
     * @return A coordinate representing the minimum value.
     */
    [[nodiscard]] coordinate getMin() const;

    /**
     * Returns the maximum value of the Parameter stored in ParameterDefinition::max.
     * @return A coordinate representing the maximum value.
     */
    [[nodiscard]] coordinate getMax() const;

    /**
     * Returns the unit string of the Parameter stored in ParameterDefinition::unit.
     * @return A string reference containing the unit.
     */
    [[nodiscard]] const string &getUnit() const;

    /**
     * Returns the configuration string of the Parameter stored in ParameterDefinition::config.
     * @return A string reference containing the configuration.
     */
    [[nodiscard]] const string &getConfig() const;
};


#endif //SIMOPTICON_PARAMETERDEFINITION_H
