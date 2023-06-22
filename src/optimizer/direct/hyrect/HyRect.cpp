#include "HyRect.h"
#include "ChildRect.h"

#include <stdexcept>

HyRect::HyRect(dimension D, position pos, depth t) : D(D), pos(pos), t(t) {
}

array<shared_ptr<HyRect>, 3> HyRect::divide(const shared_ptr<HyRect> &ptr) const {
    shared_ptr<HyRect> left(new ChildRect(D, position::LEFT, ptr));
    shared_ptr<HyRect> mid(new ChildRect(D, position::MIDDLE, ptr));
    shared_ptr<HyRect> right(new ChildRect(D, position::RIGHT, ptr));
    return {left, mid, right};
}

dirCoordinate HyRect::getDiagonalLength() const {
    dimension div = t / D;
    dimension mod = t % D;
    coordinate longSide = pow((coordinate) 3, -div);
    return sqrt((D - mod) * pow(longSide, 2) + mod * pow(longSide / 3, 2));
}

dimension HyRect::getSplitDim() const {
    return t % D + 1;
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
    return rect.pos == pos && rect.D == D;
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

dimension HyRect::getD() const {
    return D;
}
