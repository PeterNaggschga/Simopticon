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

using namespace std;

/**
 * A class used for transforming parameters between the actual Parameter space and the unit hypercube used in DIRECT algorithm.
 * @ingroup direct
 */
class ParameterNormalizer {
private:
    /**
     * ParameterDefinition of the optimized parameters.
     */
    list<shared_ptr<ParameterDefinition>> parameters;

public:
    /**
     * Creates a ParameterNormalizer with the given optimized parameters.
     * @param parameters: ParameterDefinition of the optimized parameters.
     */
    explicit ParameterNormalizer(list<shared_ptr<ParameterDefinition>> parameters);

    /**
     * Transforms the given Parameter combination into a point in the unit hypercube.
     * @param params: Parameter combination to be transformed.
     * @return A point in the unit hypercube corresponding to the given Parameter combination.
     */
    static vector<dirCoordinate> normalize(const vector<shared_ptr<Parameter>> &params);

    /**
     * Transforms the given point in the unit hypercube into a Parameter combination.
     * @param cords: Point in the unit hypercube to be transformed.
     * @return A Parameter combination corresponding to the given point in the unit hypercube.
     */
    vector<shared_ptr<Parameter>> denormalize(vector<dirCoordinate> cords);
};


#endif //SIMOPTICON_PARAMETERNORMALIZER_H
