#include "HyRect.h"

#include <stdexcept>

HyRect::HyRect(unsigned char D, position pos, HyRect *parent) : D(D), pos(pos), parent(parent),
                                                                depth(parent ? parent->depth + 1 : 0) {
}

array<HyRect *, 3> HyRect::divide(unsigned char dimension) {
    if (split != 0) {
        throw logic_error("HyRect is already divided!");
    }
    if (dimension == 0 || dimension > D) {
        throw invalid_argument("Dimension does not exist!");
    }
    split = dimension;
    auto *left = new HyRect(D, position::LEFT, this);
    auto *mid = new HyRect(D, position::MIDDLE, this);
    auto *right = new HyRect(D, position::RIGHT, this);
    return {left, mid, right};
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

array<vector<double>, 2> HyRect::getSamplingVertices() {
    if (depth == 0) {
        vector<double> a(D, 0);
        vector<double> b(D, 1);
        return {a, b};
    }
    array<vector<double>, 2> vertices = parent->getSamplingVertices();
    int psplit = parent->getDim();
    double length = vertices[1][psplit - 1] - vertices[0][psplit - 1];
    if (pos != position::RIGHT) {
        vertices[1][psplit - 1] = vertices[0][psplit - 1] + length / 3;
    }
    if (pos != position::LEFT) {
        vertices[0][psplit - 1] = vertices[0][psplit - 1] + 2 * length / 3;
    }
    if (pos == position::MIDDLE) {
        swap(vertices[0], vertices[1]);
    }
    return vertices;
}

#pragma clang diagnostic pop

unsigned char HyRect::getDim() const {
    return split;
}

position HyRect::getPos() const {
    return pos;
}

unsigned int HyRect::getDepth() const {
    return depth;
}