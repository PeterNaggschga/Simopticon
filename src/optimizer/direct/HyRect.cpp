#include "HyRect.h"

#include <stdexcept>

HyRect::HyRect(dimension D, position pos, HyRect *parent) : D(D), pos(pos), parent(parent),
                                                            t(parent ? parent->t + 1 : 0) {
}

array<HyRect, 3> HyRect::divide() {
    if (split != 0) {
        throw logic_error("HyRect is already divided!");
    }
    split = t % D + 1;
    auto left = HyRect(D, position::LEFT, this);
    auto mid = HyRect(D, position::MIDDLE, this);
    auto right = HyRect(D, position::RIGHT, this);
    return {left, mid, right};
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

array<vector<dirCoordinate>, 2> HyRect::getSamplingVerticesRecursive() {
    if (t == 0) {
        vector<dirCoordinate> a(D, 0);
        vector<dirCoordinate> b(D, 1);
        return {a, b};
    }
    array<vector<dirCoordinate>, 2> vertices = parent->getSamplingVerticesRecursive();
    dimension psplit = parent->split;
    dirCoordinate length = vertices[1][psplit - 1] - vertices[0][psplit - 1];
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

list<vector<dirCoordinate>> HyRect::getSamplingVertices() {
    list<vector<dirCoordinate>> result;
    for (const vector<dirCoordinate> &vertice: getSamplingVerticesRecursive()) {
        result.push_back(vertice);
    }
    return result;
}

dirCoordinate HyRect::getDiagonalLength() const {
    dimension div = t / D;
    dimension mod = t % D;
    coordinate longSide = pow((coordinate) 3, -div);
    return sqrt((D - mod) * pow(longSide, 2) + mod * pow(longSide / 3, 2));
}

dimension HyRect::getDim() const {
    return split;
}

position HyRect::getPos() const {
    return pos;
}

depth HyRect::getDepth() const {
    return t;
}

functionValue HyRect::getAvgValue() const {
    return avgValue;
}

void HyRect::setAvgValue(functionValue value) {
    HyRect::avgValue = value;
}

bool HyRect::operator==(const HyRect &rect) const {
    return rect.D == D && rect.t == t && rect.pos == pos && &rect.parent == &parent;
}

bool HyRect::operator<(const HyRect &rect) const {
    return t > rect.t || (t == rect.t && avgValue < rect.avgValue);
}

bool HyRect::operator!=(const HyRect &rhs) const {
    return !(rhs == *this);
}

bool HyRect::operator>(const HyRect &rhs) const {
    return rhs < *this;
}

bool HyRect::operator<=(const HyRect &rhs) const {
    return !(rhs < *this);
}

bool HyRect::operator>=(const HyRect &rhs) const {
    return !(*this < rhs);
}
