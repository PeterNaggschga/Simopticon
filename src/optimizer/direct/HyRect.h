#ifndef SIMOPTICON_HYRECT_H
#define SIMOPTICON_HYRECT_H

#include <optional>
#include <array>
#include <vector>
#include <cmath>

using namespace std;

enum class position : char {
    LEFT = 0, MIDDLE = 1, RIGHT = 2, BASE = -1
};

class HyRect {

private:
    const unsigned char D;
    unsigned int depth;
    position pos;
    unsigned char split = 0;
    HyRect *parent;
    long double value = INFINITY;

public:
    HyRect(unsigned char D, position pos, HyRect *parent);

    array<HyRect, 3> divide();

    array<vector<long double>, 2> getSamplingVertices();

    [[nodiscard]] unsigned int getDepth() const;

    [[nodiscard]] position getPos() const;

    [[nodiscard]] unsigned char getDim() const;

    [[nodiscard]] long double getValue() const;

    void setValue(long double value);

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
