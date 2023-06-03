#include "BaseRect.h"

BaseRect::BaseRect(dimension D) : HyRect(D, position::BASE, 0) {
}

array<vector<dirCoordinate>, 2> BaseRect::getSamplingVertices() {
    vector<dirCoordinate> a(D, 0);
    vector<dirCoordinate> b(D, 1);
    return {a, b};
}
