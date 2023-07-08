#ifndef SIMOPTICON_LEVELS_H
#define SIMOPTICON_LEVELS_H

#include "DirectComparisonFunctions.h"
#include "DirectTypes.h"

#include <memory>
#include <map>
#include <list>
#include <set>

class HyRect;

enum level : unsigned char {
    l2_0 = 0, l1_1 = 1, l0_2 = 2, l1_3 = 3, l1_4 = 4, l0_5 = 5, l1_6 = 6, l2_7 = 7
};

class Levels {

private:
    level currentLevel = l2_0;
    bool global = false;

public:
    constexpr static const double L3_EPSILON = 1e-5;
    constexpr static const double L2_EPSILON = 1e-5;
    constexpr static const double L1_EPSILON = 1e-7;
    constexpr static const double L0_EPSILON = 0;

    constexpr static const long double L3_SIZE = 0.5;
    constexpr static const long double L2_SIZE = 1;
    constexpr static const long double L1_SIZE = 0.9;
    const long double L0_SIZE;

    explicit Levels(long double l0Size);

    unsigned char nextLevel();

    [[nodiscard]] list<shared_ptr<HyRect>>
    getRectSubset(const map<depth, set<shared_ptr<HyRect>, CmpSharedHyrect>, greater<>> &rects, size_t size) const;

    [[nodiscard]] double getEpsilon() const;

    [[nodiscard]] unsigned char getLevel() const;

    [[nodiscard]] bool isGlobal() const;

    void setGlobal(bool val);
};


#endif //SIMOPTICON_LEVELS_H
