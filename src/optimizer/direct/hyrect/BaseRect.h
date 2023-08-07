#ifndef SIMOPTICON_BASERECT_H
#define SIMOPTICON_BASERECT_H

/**
 * @file
 * In this file, the header of the BaseRect class is defined.
 */

#include "HyRect.h"

/**
 * A class representing a HyRect without a parent rectangle.
 * This rectangle is always at the root of a partition tree and therefore has depth @f$ t=0 @f$ and represents the whole search space.
 * @ingroup hyrect
 */
class BaseRect : public HyRect {
public:
    /**
     * Creates a BaseRect representing a hypercube with the given dimensionality.
     * @param D: Number of dimensions of the search space.
     */
    explicit BaseRect(dimension D);

    /**
     * Returns the coordinates of two opposite corner points of the rectangle.
     * The returned vertices must be sampled.
     * For BaseRect always returns one vector full of zeros and one vector full of ones.
     * @return An array containing two dirCoordinate vectors of the sampled vertices.
     */
    std::array<std::vector<dirCoordinate>, 2> getSamplingVertices() override;
};


#endif //SIMOPTICON_BASERECT_H
