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

array<vector<coordinate>, 2> HyRect::getSamplingVertices() {
    if (t == 0) {
        vector<coordinate> a(D, 0);
        vector<coordinate> b(D, 1);
        return {a, b};
    }
    array<vector<coordinate>, 2> vertices = parent->getSamplingVertices();
    dimension psplit = parent->split;
    coordinate length = vertices[1][psplit - 1] - vertices[0][psplit - 1];
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

dimension HyRect::getDim() const {
    return split;
}

position HyRect::getPos() const {
    return pos;
}

depth HyRect::getDepth() const {
    return t;
}

functionValue HyRect::getValue() const {
    return value;
}

void HyRect::setValue(functionValue value) {
    HyRect::value = value;
}

bool HyRect::operator==(const HyRect &rect) const {
    return rect.D == D && rect.t == t && rect.pos == pos && &rect.parent == &parent;
}

bool HyRect::operator<(const HyRect &rect) const {
    return t > rect.t || (t == rect.t && value < rect.value);
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
