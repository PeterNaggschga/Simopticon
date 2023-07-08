#include "Levels.h"

#include "hyrect/HyRect.h"

#include <cmath>
#include <ranges>

Levels::Levels(long double l0Size) : L0_SIZE(l0Size) {
}

unsigned char Levels::nextLevel() {
    if (!global) {
        currentLevel = static_cast<level>(((int) currentLevel + 1) % 8);
    }
    return getLevel();
}

list<shared_ptr<HyRect>>
Levels::getRectSubset(const map<depth, set<shared_ptr<HyRect>, CmpSharedHyrect>, greater<>> &rects, size_t size) const {
    switch (getLevel()) {
        case 2:
            size = ceil(size * L2_SIZE);
            break;
        case 1:
            size = ceil(size * L1_SIZE);
            break;
        case 0:
            size = ceil(size * L0_SIZE);
            break;
        default:
            size = ceil(size * L3_SIZE);
    }
    list<shared_ptr<HyRect>> result;
    if (global) {
        for (const auto &entry: std::views::reverse(rects)) {
            result.emplace_back(*entry.second.begin());
            if (size <= entry.second.size()) {
                break;
            } else {
                size -= entry.second.size();
            }
        }
    } else {
        for (const auto &entry: rects) {
            result.emplace_back(*entry.second.begin());
            if (size <= entry.second.size()) {
                break;
            } else {
                size -= entry.second.size();
            }
        }
    }
    return result;
}

double Levels::getEpsilon() const {
    switch (getLevel()) {
        case 2:
            return L2_EPSILON;
        case 1:
            return L1_EPSILON;
        case 0:
            return L0_EPSILON;
        default:
            return L3_EPSILON;
    }
}

unsigned char Levels::getLevel() const {
    if (global) {
        return 3;
    }
    switch (currentLevel) {
        case l2_0:
        case l2_7:
            return 2;
        case l0_2:
        case l0_5:
            return 0;
        default:
            return 1;
    }
}

bool Levels::isGlobal() const {
    return global;
}

void Levels::setGlobal(bool val) {
    if (val != global) {
        currentLevel = level::l2_0;
    }
    global = val;
}
