#ifndef SIMOPTICON_HYRECT_H
#define SIMOPTICON_HYRECT_H

#include "../../Types.h"
#include "DirectTypes.h"

#include <optional>
#include <array>
#include <vector>
#include <cmath>
#include <list>

using namespace std;

enum class position : char {
    LEFT = 0, MIDDLE = 1, RIGHT = 2, BASE = -1
};

class HyRect {

private:
    const dimension D;
    depth t;
    position pos;
    dimension split = 0;
    HyRect *parent;
    functionValue value = INFINITY;

    array<vector<dirCoordinate>, 2> getSamplingVerticesRecursive();

public:
    HyRect(dimension D, position pos, HyRect *parent);

    array<HyRect, 3> divide();

    list<vector<dirCoordinate>> getSamplingVertices();

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
