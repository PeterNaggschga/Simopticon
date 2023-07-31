#include "ValueMap.h"

#include <stdexcept>
#include <cmath>
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
        advance(it, floor((float) diff / 2));
        upperValues.insert(it, lowerValues.end());
        lowerValues.erase(it, lowerValues.end());
    }
    operationsLock.unlock();
}

void ValueMap::addValue(const pair<vector<shared_ptr<Parameter>>, functionValue> &val,
                        set<functionValue *, CmpPtrFunctionvalue> &set) {
    auto worked = values.insert(val);
    if (!worked.second) {
        throw logic_error("Value couldn't be inserted!");
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

functionValue ValueMap::query(const vector<shared_ptr<Parameter>> &params) {
    updateMap();
    operationsLock.lock();
    auto res = values.find(params);
    if (res == values.end()) {
        throw invalid_argument("Param combination not present!");
    }
    operationsLock.unlock();
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

list<pair<vector<shared_ptr<Parameter>>, functionValue>> ValueMap::getTopVals() {
    updateMap();
    return {topVals.begin(), topVals.end()};
}

bool ValueMap::isTopValue(const vector<shared_ptr<Parameter>> &cords) {
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

const map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> &ValueMap::getValues() {
    updateMap();
    return values;
}
