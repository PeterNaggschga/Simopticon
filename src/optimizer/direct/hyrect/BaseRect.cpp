/**
 * @file
 * In this file, the implementation of the BaseRect class is defined.
 */

#include "BaseRect.h"

BaseRect::BaseRect(dimension D) : HyRect(D, position::BASE, 0) {
}

std::array<std::vector<dirCoordinate>, 2> BaseRect::getSamplingVertices() {
    std::vector<dirCoordinate> a(D, 0);
    std::vector<dirCoordinate> b(D, 1);
    return {a, b};
}
