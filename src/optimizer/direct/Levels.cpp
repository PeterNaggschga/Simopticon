#include "Levels.h"

level Levels::nextLevel() {
    currentLevel = static_cast<level>(((int) currentLevel + 1) % 8);
    return currentLevel;
}

size_t Levels::getRectSubsetSize(size_t size) {
    switch (currentLevel) {
        case l1_1:
        case l1_3:
        case l1_4:
        case l1_6:
            return size * 9 / 10 + 1;
        case l0_2:
        case l0_5:
            return size / 10 + 1;
        default:
            return size;
    }
}

double Levels::getEpsilon() {
    switch (currentLevel) {
        case l1_1:
        case l1_3:
        case l1_4:
        case l1_6:
            return L1_EPSILON;
        case l0_2:
        case l0_5:
            return L0_EPSILON;
        default:
            return L2_EPSILON;
    }
}
