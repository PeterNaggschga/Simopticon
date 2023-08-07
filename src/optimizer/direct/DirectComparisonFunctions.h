#ifndef SIMOPTICON_DIRECTCOMPARISONFUNCTIONS_H
#define SIMOPTICON_DIRECTCOMPARISONFUNCTIONS_H

#include "hyrect/HyRect.h"

/**
 * In this file, comparison functions are defined which are used in the direct module.
 * They can be used to order elements in STL containers.
 *
 * @file DirectComparisonFunctions.h
 * @ingroup direct
 */

/**
 * This struct implements the comparison of two shared pointers to HyRect instances.
 */
struct CmpSharedHyrect {
    /**
     * Compares two shared pointers to HyRect instances.
     * @param a: First pointer to a HyRect.
     * @param b: Second pointer to a HyRect.
     * @return True if a has a lower HyRect#avgValue value than b. If both values are the same, compare the sampling vertices returned by HyRect#getSamplingVertices.
     */
    bool operator()(const shared_ptr<HyRect> &a, const shared_ptr<HyRect> &b) const {
        return a->getAvgValue() == b->getAvgValue() ? a->getSamplingVertices() < b->getSamplingVertices() :
               a->getAvgValue() < b->getAvgValue();
    }
};

#endif //SIMOPTICON_DIRECTCOMPARISONFUNCTIONS_H
