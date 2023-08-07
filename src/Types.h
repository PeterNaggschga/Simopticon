#ifndef SIMOPTICON_TYPES_H
#define SIMOPTICON_TYPES_H

#include <string>

#include <vector>
#include <memory>

class Parameter;

/**
 * In this file, types are defined which should be used across the whole framework.
 *
 * @file Types.h
 * @ingroup utils
 */

/**
 * A complex type representing a vector in parameter space.
 */
typedef std::vector<std::shared_ptr<Parameter>> parameterCombination;

/**
 * A floating point type containing the value of an optimized function.
 */
typedef long double functionValue;

/**
 * A floating point type used to represent Parameter values
 */
typedef double coordinate;

/**
 * An identifier that makes different simulation runs in one result file folder distinguishable.
 * Uniqueness is not being asserted.
 */
typedef std::string runId;

#endif //SIMOPTICON_TYPES_H
