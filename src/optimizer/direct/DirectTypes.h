#ifndef SIMOPTICON_DIRECTTYPES_H
#define SIMOPTICON_DIRECTTYPES_H

/**
 * In this file, types are defined which are used in the direct module.
 *
 * @file DirectTypes.h
 * @ingroup direct
 */

/**
 * An integral type used for representing the depth of a HyRect in the partition tree.
 */
typedef unsigned int depth;

/**
 * An integral type used for representing a dimension of the search space.
 * Please note that the first dimension is represented by value 1, not 0.
 */
typedef unsigned char dimension;

/**
 * A floating point type used for representing one coordinate in the hypercube search space.
 */
typedef long double dirCoordinate;

#endif //SIMOPTICON_DIRECTTYPES_H
