#ifndef SIMOPTICON_PARAMETERNORMALIZER_H
#define SIMOPTICON_PARAMETERNORMALIZER_H

/**
 * @file
 * In this file, the header of the ParameterNormalizer class is defined.
 */

#include "DirectTypes.h"
#include "../../Types.h"

#include <list>
#include <vector>
#include <memory>

class ParameterDefinition;

class Parameter;

/**
 * A class used for transforming parameters between the actual Parameter space and the unit hypercube used in DIRECT algorithm.
 * @ingroup direct
 */
class ParameterNormalizer {
private:
    /**
     * ParameterDefinition of the optimized parameters.
     */
    std::list<std::shared_ptr<ParameterDefinition>> parameters;

public:
    /**
     * Creates a ParameterNormalizer with the given optimized parameters.
     * @param parameters: ParameterDefinition of the optimized parameters.
     */
    explicit ParameterNormalizer(std::list<std::shared_ptr<ParameterDefinition>> parameters);

    /**
     * Transforms the given parameterCombination into a point in the unit hypercube.
     * @param params: parameterCombination to be transformed.
     * @return A point in the unit hypercube corresponding to the given parameterCombination.
     */
    static std::vector<dirCoordinate> normalize(const parameterCombination &params);

    /**
     * Transforms the given point in the unit hypercube into a parameterCombination.
     * @param cords: Point in the unit hypercube to be transformed.
     * @return A parameterCombination corresponding to the given point in the unit hypercube.
     */
    parameterCombination denormalize(std::vector<dirCoordinate> cords);
};


#endif //SIMOPTICON_PARAMETERNORMALIZER_H
