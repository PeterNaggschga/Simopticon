#include "HyRect.h"

#include <stdexcept>

HyRect::HyRect(unsigned char D, position pos, HyRect *parent) : D(D), pos(pos), parent(parent),
                                                                depth(parent ? parent->depth + 1 : 0) {
}

array<HyRect, 3> HyRect::divide() {
    if (split != 0) {
        throw logic_error("HyRect is already divided!");
    }
    split = depth % D + 1;
    auto left = HyRect(D, position::LEFT, this);
    auto mid = HyRect(D, position::MIDDLE, this);
    auto right = HyRect(D, position::RIGHT, this);
    return {left, mid, right};
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

array<vector<long double>, 2> HyRect::getSamplingVertices() {
    if (depth == 0) {
        vector<long double> a(D, 0);
        vector<long double> b(D, 1);
        return {a, b};
    }
    auto vertices = parent->getSamplingVertices();
    auto psplit = parent->split;
    auto length = vertices[1][psplit - 1] - vertices[0][psplit - 1];
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

unsigned char HyRect::getDim() const {
    return split;
}

position HyRect::getPos() const {
    return pos;
}

unsigned int HyRect::getDepth() const {
    return depth;
}

long double HyRect::getValue() const {
    return value;
}

void HyRect::setValue(long double value) {
    HyRect::value = value;
}

bool HyRect::operator==(const HyRect &rect) const {
    return rect.D == D && rect.depth == depth && rect.pos == pos && &rect.parent == &parent;
}

bool HyRect::operator<(const HyRect &rect) const {
    return depth > rect.depth || (depth == rect.depth && value < rect.value);
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
