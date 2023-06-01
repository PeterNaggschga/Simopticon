#include "ValueMap.h"

#include "../parameters/Parameter.h"

#include <stdexcept>

void ValueMap::updateMap() {
    if (tba.empty()) {
        return;
    }
    if (upperValues.empty()) {
        auto entry = tba.begin();
        auto worked = values.insert(*entry);
        if (!worked.second) {
            throw logic_error("Value couldn't be inserted!");
        }
        upperValues.insert(&worked.first->second);
        tba.erase(entry);
    }

    for (const auto &entry: tba) {
        auto worked = values.insert(entry);
        if (!worked.second) {
            throw logic_error("Value couldn't be inserted!");
        }
        if (entry.second < **upperValues.begin()) {
            lowerValues.insert(&worked.first->second);
        } else {
            upperValues.insert(&worked.first->second);
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
        advance(it, diff / 2 - diff % 2);
        upperValues.insert(it, lowerValues.end());
        lowerValues.erase(it, lowerValues.end());
    }
}

functionValue ValueMap::query(const vector<shared_ptr<Parameter>> &params) {
    updateMap();
    auto res = values.find(params);
    if (res == values.end()) {
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
    return values.find(cords) != values.end();
}

functionValue ValueMap::getMedian() {
    if (getSize() == 0) {
        return 0;
    }
    updateMap();
    if (lowerValues.size() == upperValues.size()) {
        auto it = lowerValues.end();
        advance(it, -1);
        return (**it + **upperValues.begin()) / 2;
    }
    return **upperValues.begin();
}

unsigned long ValueMap::getSize() const {
    return values.size() + tba.size();
}

