#include "ValueMap.h"

#include "../parameters/Parameter.h"

#include <stdexcept>

void ValueMap::updateMap() {
    if (tba.empty()) {
        return;
    }
    if (lowerValues.empty()) {
        auto entry = tba.begin();
        lowerValues.insert(*entry);
        tba.erase(entry);
    }

    for (const auto &entry: tba) {
        if (entry.first < lowerValues.begin()->first) {
            upperValues.insert(entry);
        } else {
            lowerValues.insert(entry);
        }
    }
    tba.clear();

    long diff = (long) upperValues.size() - (long) lowerValues.size();
    if (diff < 0) {
        auto it = lowerValues.begin();
        advance(it, -diff / 2);
        upperValues.insert(lowerValues.begin(), it);
        lowerValues.erase(lowerValues.begin(), it);
    } else {
        auto it = upperValues.end();
        advance(it, -(diff / 2 + diff % 2));
        lowerValues.insert(it, upperValues.end());
        upperValues.erase(it, upperValues.end());
    }
}

functionValue ValueMap::query(const vector<shared_ptr<Parameter>> &params) {
    updateMap();
    auto *map = params < lowerValues.begin()->first ? &upperValues : &lowerValues;
    auto res = map->find(params);
    if (res == map->end()) {
        throw invalid_argument("Param combination not present!");
    }
    return res->second;
}

void ValueMap::insert(const vector<shared_ptr<Parameter>> &params, functionValue val) {
    tba.emplace_back(params, val);
}

bool ValueMap::isKnown(const vector<shared_ptr<Parameter>> &cords) {
    if (getSize() == 0) {
        return false;
    }
    updateMap();
    auto *map = cords < lowerValues.begin()->first ? &upperValues : &lowerValues;
    return map->find(cords) != map->end();
}

functionValue ValueMap::getMedian() {
    updateMap();
    if (getSize() == 0) {
        return 0;
    }
    if (lowerValues.size() == upperValues.size()) {
        auto it = upperValues.end();
        advance(it, -1);
        return (it->second + lowerValues.begin()->second) / 2;
    }
    return lowerValues.begin()->second;
}

unsigned long ValueMap::getSize() const {
    return upperValues.size() + lowerValues.size() + tba.size();
}

