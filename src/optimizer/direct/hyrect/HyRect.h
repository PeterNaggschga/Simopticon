#ifndef SIMOPTICON_HYRECT_H
#define SIMOPTICON_HYRECT_H

#include "../../../Types.h"
#include "../DirectTypes.h"

#include <optional>
#include <array>
#include <vector>
#include <cmath>
#include <list>
#include <memory>

using namespace std;

enum class position : char {
    LEFT = 0, MIDDLE = 1, RIGHT = 2, BASE = -1
};

class HyRect {

protected:
    dimension D;
    depth t;
    position pos;
    dimension split = 0;
    functionValue avgValue = INFINITY;

public:
    HyRect(dimension D, position pos, depth t);

    array<shared_ptr<HyRect>, 3> divide(const shared_ptr<HyRect> &ptr);

    virtual array<vector<dirCoordinate>, 2> getSamplingVertices() = 0;

    [[nodiscard]] dirCoordinate getDiagonalLength() const;

    [[nodiscard]] ::depth getDepth() const;

    [[nodiscard]] position getPos() const;

    [[nodiscard]] ::dimension getSplitDim() const;

    [[nodiscard]] functionValue getAvgValue() const;

    [[nodiscard]] dimension getD() const;

    void setAvgValue(functionValue value);

    virtual bool operator==(const HyRect &rect) const;

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
            return hash<double>()(
                    pow((int) x.getPos() + 2, x.getSplitDim() * x.getDepth() + 1) + (double) x.getAvgValue());
        }
    };
}

#endif //SIMOPTICON_HYRECT_H
