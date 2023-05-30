#ifndef SIMOPTICON_HYRECT_H
#define SIMOPTICON_HYRECT_H

#include "../../Types.h"

#include <optional>
#include <array>
#include <vector>
#include <cmath>

using namespace std;

typedef unsigned int depth;

typedef unsigned char dimension;

typedef long double coordinate;

enum class position : char {
    LEFT = 0, MIDDLE = 1, RIGHT = 2, BASE = -1
};

class HyRect {

private:
    const dimension D;
    depth depth;
    position pos;
    dimension split = 0;
    HyRect *parent;
    functionValue value = INFINITY;

public:
    HyRect(dimension D, position pos, HyRect *parent);

    array<HyRect, 3> divide();

    array<vector<coordinate>, 2> getSamplingVertices();

    [[nodiscard]] ::depth getDepth() const;

    [[nodiscard]] position getPos() const;

    [[nodiscard]] ::dimension getDim() const;

    [[nodiscard]] functionValue getValue() const;

    void setValue(functionValue value);

    bool operator==(const HyRect &rect) const;

    bool operator<(const HyRect &rect) const;

    bool operator!=(const HyRect &rhs) const;

    bool operator>(const HyRect &rhs) const;

    bool operator<=(const HyRect &rhs) const;

    bool operator>=(const HyRect &rhs) const;
};

namespace std {
    template<>
    struct [[maybe_unused]] hash<HyRect> {
        size_t operator()(const HyRect &x) const {
            return hash<double>()(pow((int) x.getPos() + 2, x.getDim() * x.getDepth() + 1) + (double) x.getValue());
        }
    };
}

#endif //SIMOPTICON_HYRECT_H
