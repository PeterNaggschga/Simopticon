#ifndef SIMOPTICON_GRAHAMSCAN_H
#define SIMOPTICON_GRAHAMSCAN_H

/**
 * @file
 * In this file the header of the GrahamScan class is defined.
 */

#include <list>
#include <memory>

class HyRect;

using namespace std;

/**
 * A class providing functionality for finding the lower right convex hull of a set of points.
 * @ingroup direct
 */
class GrahamScan {
public:
    /**
     * Calculates the lower right convex hull of a set of points.
     * Points are defined by the given HyRects diagonal length (x axis) and average value (y axis).
     * For each returned HyRect the slope to the point right of it is returned (if it is the rightmost point, infinity is chosen).
     * That slope value can be used by DIRECT as the highest Lipschitz constant for which the HyRect satisfies the first condition.
     * @param vertices: List of rectangles with different sizes.
     * @return A list of rectangles and corresponding Lipschitz constants that represents convex hull meaning a subset of the given HyRect list.
     */
    static list<pair<shared_ptr<HyRect>, double>> scan(list<shared_ptr<HyRect>> vertices);
};


#endif //SIMOPTICON_GRAHAMSCAN_H
