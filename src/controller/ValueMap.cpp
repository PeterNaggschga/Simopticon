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
    topEntriesChanged = false;
}

void
ValueMap::addValue(const pair<vector<shared_ptr<Parameter>>, functionValue> &val,
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
    auto res = values.find(params);
    if (res == values.end()) {
        throw invalid_argument("Param combination not present!");
    }
    return res->second;
}

void ValueMap::insert(const vector<shared_ptr<Parameter>> &params, functionValue val) {
    if (topVals.empty() || val < topVals.rend()->second) {
        topEntriesChanged = true;
    }
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

size_t ValueMap::getSize() const {
    return values.size() + tba.size();
}

list<pair<vector<shared_ptr<Parameter>>, functionValue>> ValueMap::getTopVals() {
    if (topEntriesChanged) {
        updateMap();
    }
    return {topVals.begin(), topVals.end()};
}

bool ValueMap::isTopValue(const vector<shared_ptr<Parameter>> &cords) {
    if (topEntriesChanged) {
        updateMap();
    }
    auto pred = [&cords](const pair<const vector<shared_ptr<Parameter>>, functionValue> &p) {
        if (p.first.size() != cords.size()) {
            return false;
        }
        for (int i = 0; i < p.first.size(); ++i) {
            if (p.first[i]->getVal() != cords[i]->getVal()) {
                return false;
            }
        }
        return true;
    };
    return ranges::any_of(topVals, pred);
}
