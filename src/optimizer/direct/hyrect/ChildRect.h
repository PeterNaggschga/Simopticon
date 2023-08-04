#ifndef SIMOPTICON_CHILDRECT_H
#define SIMOPTICON_CHILDRECT_H


#include "HyRect.h"

/**
 * A class representing a HyRect that has a parent HyRect.
 * Used for all HyRect where depth @f$ t > 0 @f$.
 * @ingroup hyrect
 */
class ChildRect : public HyRect {
private:
    /**
     * Reference to the parent rectangle. Used for recursive calculation of ChildRect::getSamplingVertices.
     */
    shared_ptr<HyRect> parent;

public:
    /**
     * Creates a ChildRect with the given relative position and parent rectangle.
     * @param pos: Relative position to the given parent rectangle.
     * @param parent: Parent rectangle in the partition tree.
     */
    ChildRect(position pos, shared_ptr<HyRect> parent);

    /**
     * Returns the coordinates of two opposite corner points of the rectangle.
     * The returned vertices must be sampled.
     * The vertices are calculated recursively based on the sampling vertices of ChildRect::parent.
     * @return An array containing two dirCoordinate vectors of the sampled vertices.
     */
    array<vector<dirCoordinate>, 2> getSamplingVertices() override;

    bool operator==(const HyRect &rect) const override;
};


#endif //SIMOPTICON_CHILDRECT_H
