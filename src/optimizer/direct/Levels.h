#ifndef SIMOPTICON_LEVELS_H
#define SIMOPTICON_LEVELS_H

enum level : unsigned char {
    l2_0 = 0, l1_1 = 1, l0_2 = 2, l1_3 = 3, l1_4 = 4, l0_5 = 5, l1_6 = 6, l2_7 = 7
};

class Levels {

private:
    level currentLevel = l2_0;

public:
    constexpr static const double L2_EPSILON = 0.00001;
    constexpr static const double L1_EPSILON = 0.0000001;
    constexpr static const double L0_EPSILON = 0;

    Levels() = default;

    level nextLevel();

    unsigned long getRectSubsetSize(unsigned long size);

    [[nodiscard]] double getEpsilon();
};


#endif //SIMOPTICON_LEVELS_H
