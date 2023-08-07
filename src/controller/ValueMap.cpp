/**
 * @file
 * In this file, the implementation of the ValueMap class is defined.
 */

#include "ValueMap.h"

#include <cmath>
#include <stdexcept>
#include <algorithm>

ValueMap::ValueMap(unsigned int topEntries) : topEntries(topEntries) {
}

void ValueMap::updateMap() {
    if (tba.empty()) {
        return;
    }
    operationsLock.lock();
    if (upperValues.empty()) {
        auto entry = tba.begin();
        addValue(*entry, upperValues);
        tba.erase(entry);
    }

    for (const auto &entry: tba) {
        if (entry.second < **upperValues.begin()) {
            addValue(entry, lowerValues);
        } else {
            addValue(entry, upperValues);
        }
    }
    tba.clear();

    long diff = (long) upperValues.size() - (long) lowerValues.size();
    if (diff > 0) {
        auto it = upperValues.begin();
        advance(it, diff / 2);
        lowerValues.insert(upperValues.begin(), it);
        upperValues.erase(upperValues.begin(), it);
    } else if (diff < 0) {
        auto it = lowerValues.end();
        advance(it, std::floor((float) diff / 2));
        upperValues.insert(it, lowerValues.end());
        lowerValues.erase(it, lowerValues.end());
    }
    operationsLock.unlock();
}

void ValueMap::addValue(const std::pair<parameterCombination, functionValue> &val,
                        std::set<functionValue *, CmpPtrFunctionvalue> &set) {
    auto worked = values.insert(val);
    if (!worked.second) {
        throw std::logic_error("Value couldn't be inserted!");
    }
    set.insert(&worked.first->second);
    if (topVals.empty()) {
        topVals.insert(val);
        return;
    }
    auto it = topVals.rbegin();
    if (topVals.size() < topEntries || val.second < it->second) {
        topVals.insert(val);
        if (topVals.size() > topEntries) {
            it = topVals.rbegin();
            topVals.erase(*it);
        }
    }
}

functionValue ValueMap::query(const parameterCombination &params) {
    updateMap();
    operationsLock.lock();
    auto res = values.find(params);
    if (res == values.end()) {
        throw std::invalid_argument("Param combination not present!");
    }
    operationsLock.unlock();
    return res->second;
}

void ValueMap::insert(const parameterCombination &params, functionValue val) {
    tba.emplace_back(params, val);
}

bool ValueMap::isKnown(const parameterCombination &cords) {
    if (getSize() == 0) {
        return false;
    }
    updateMap();
    operationsLock.lock();
    bool res = values.find(cords) != values.end();
    operationsLock.unlock();
    return res;
}

functionValue ValueMap::getMedian() {
    if (getSize() == 0) {
        return 0;
    }
    updateMap();
    operationsLock.lock();
    functionValue res = lowerValues.size() == upperValues.size() ? (**lowerValues.rbegin() + **upperValues.begin()) / 2
                                                                 : **upperValues.begin();
    operationsLock.unlock();
    return res;
}

size_t ValueMap::getSize() const {
    return values.size() + tba.size();
}

std::list<std::pair<parameterCombination, functionValue>> ValueMap::getTopVals() {
    updateMap();
    return {topVals.begin(), topVals.end()};
}

bool ValueMap::isTopValue(const parameterCombination &cords) {
    updateMap();
    for (const auto &entry: topVals) {
        if (entry.first.size() != cords.size()) {
            continue;
        }
        for (int i = 0; i < entry.first.size(); ++i) {
            if (entry.first[i]->getVal() != cords[i]->getVal()) {
                continue;
            }
        }
        return true;
    }
    return false;
}

const std::map<parameterCombination, functionValue, CmpVectorSharedParameter> &ValueMap::getValues() {
    updateMap();
    return values;
}
