/**
 * @file
 * In this file the implementation of the BaseRect class is defined.
 */

#include "BaseRect.h"

BaseRect::BaseRect(dimension D) : HyRect(D, position::BASE, 0) {
}

array<vector<dirCoordinate>, 2> BaseRect::getSamplingVertices() {
    vector<dirCoordinate> a(D, 0);
    vector<dirCoordinate> b(D, 1);
    return {a, b};
}
