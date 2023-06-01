#include "BaseRect.h"

BaseRect::BaseRect(dimension D) : HyRect(D, position::BASE, 0) {
}

array<vector<dirCoordinate>, 2> BaseRect::getSamplingVertices() {
    vector<dirCoordinate> a(getD(), 0);
    vector<dirCoordinate> b(getD(), 1);
    return {a, b};
}
