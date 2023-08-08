/**
 * @file
 * In this file, the implementation of the Levels class is defined.
 */

#include "Levels.h"

#include "hyrect/HyRect.h"

#include <cmath>

unsigned char Levels::nextLevel() {
    if (!global) {
        currentLevel = static_cast<level>(((int) currentLevel + 1) % 8);
    }
    return getLevel();
}

std::list<std::shared_ptr<HyRect>>
Levels::getRectSubset(const std::map<depth, std::set<std::shared_ptr<HyRect>, CmpSharedHyrect>, std::greater<>> &rects,
                      size_t size) const {
    switch (getLevel()) {
        case 2:
            size = std::ceil(size * L2_SIZE);
            break;
        case 1:
            size = std::ceil(size * L1_SIZE);
            break;
        case 0:
            size = std::ceil(size * L0_SIZE);
            break;
        default:
            size = std::ceil(size * L3_SIZE);
    }
    std::list<std::shared_ptr<HyRect>> result;
    if (global) {
        for (auto it = rects.rbegin(); it != rects.rend(); ++it) {
            result.push_back(*it->second.begin());
            if (size <= it->second.size()) {
                break;
            } else {
                size -= it->second.size();
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
    global = val;
}
