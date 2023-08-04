#ifndef SIMOPTICON_LEVELS_H
#define SIMOPTICON_LEVELS_H

#include "DirectComparisonFunctions.h"
#include "DirectTypes.h"

#include <memory>
#include <map>
#include <list>
#include <set>

class HyRect;

/**
 * An enum representing the sequence of local levels.
 * @ingroup direct
 */
enum level : unsigned char {
    l2_0 = 0, l1_1 = 1, l0_2 = 2, l1_3 = 3, l1_4 = 4, l0_5 = 5, l1_6 = 6, l2_7 = 7
};

/**
 * A providing functionality for the usage of different weightings between local and global search throughout the optimization using different levels.
 * @ingroup direct
 */
class Levels {
private:
    /**
     * Local level the optimization is currently using when Levels::global is @a false.
     */
    level currentLevel = l2_0;
    /**
     * Defines whether global optimization (level 3) or one of the local levels (0-2) is used.
     */
    bool global = false;

public:
    /**
     * Epsilon value to be used when DIRECT algorithm uses level 3.
     */
    constexpr static const double L3_EPSILON = 1e-5;
    /**
     * Epsilon value to be used when DIRECT algorithm uses level 2.
     */
    constexpr static const double L2_EPSILON = 1e-5;
    /**
     * Epsilon value to be used when DIRECT algorithm uses level 1.
     */
    constexpr static const double L1_EPSILON = 1e-7;
    /**
     * Epsilon value to be used when DIRECT algorithm uses level 0.
     */
    constexpr static const double L0_EPSILON = 0;

    /**
     * Fraction of rectangles in partition to be used on level 3 (only larger rectangles are considered).
     */
    constexpr static const long double L3_SIZE = 0.5;
    /**
     * Fraction of rectangles in partition to be used on level 2 (only smaller rectangles are considered).
     */
    constexpr static const long double L2_SIZE = 1;
    /**
     * Fraction of rectangles in partition to be used on level 1 (only smaller rectangles are considered).
     */
    constexpr static const long double L1_SIZE = 0.95;
    /**
     * Fraction of rectangles in partition to be used on level 0 (only smaller rectangles are considered).
     */
    constexpr static const long double L0_SIZE = 0.04;

    Levels() = default;

    /**
     * Switches Levels::currentLevel to the next local level if Levels::global is false.
     * @return A number representing the current level after switching.
     */
    unsigned char nextLevel();

    /**
     * Calculates the subset of all given rectangles based on the current level and returns a list containing only the best HyRect per diagonal length.
     * @param rects: Map containing all HyRect of the current partition grouped by HyRect::t and sorted by HyRect::avgValue.
     * @param size: Number of HyRect in the given partition.
     * @return A list containing only the best HyRect per diagonal length in the subset based on the current level.
     */
    [[nodiscard]] list<shared_ptr<HyRect>>
    getRectSubset(const map<depth, set<shared_ptr<HyRect>, CmpSharedHyrect>, greater<>> &rects, size_t size) const;

    /**
     * Returns the epsilon value on the current level the DIRECT algorithm resides on.
     * Either Levels::L3_EPSILON, Levels::L2_EPSILON, Levels::L1_EPSILON or Levels::L0_EPSILON.
     * @return A floating point value used as epsilon parameter on the current level.
     */
    [[nodiscard]] double getEpsilon() const;

    /**
     * Returns a number corresponding to the current level the optimization resides on.
     * @return An integral corresponding to the current level.
     */
    [[nodiscard]] unsigned char getLevel() const;

    /**
     * Returns the value of Levels::global.
     * @return A boolean defining whether the optimization is currently in the global phase.
     */
    [[nodiscard]] bool isGlobal() const;

    /**
     * Sets the value of Levels::global.
     * @param val: Defines whether global optimization should be used in the following iterations.
     */
    void setGlobal(bool val);
};


#endif //SIMOPTICON_LEVELS_H
