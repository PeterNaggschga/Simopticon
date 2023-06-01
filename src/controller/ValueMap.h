#ifndef SIMOPTICON_VALUEMAP_H
#define SIMOPTICON_VALUEMAP_H


#include "../Types.h"

#include <map>
#include <vector>
#include <list>
#include <memory>
#include <set>

class Parameter;

using namespace std;

class ValueMap {
private:
    struct PtrCmp {
        bool operator()(const functionValue *a, const functionValue *b) const { return *a == *b ? a < b : *a < *b; }
    };

    set<functionValue *, PtrCmp> upperValues;
    set<functionValue *, PtrCmp> lowerValues;

    struct PairCmp {
        bool operator()(const pair<vector<shared_ptr<Parameter>>, functionValue> &a,
                        const pair<vector<shared_ptr<Parameter>>, functionValue> &b) const {
            return a.second == b.second ? a.first < b.first : a.second < b.second;
        }
    };

    unsigned int topEntries;
    set<pair<const vector<shared_ptr<Parameter>>, functionValue>, PairCmp> topVals;

    map<vector<shared_ptr<Parameter>>, functionValue> values;
    list<pair<vector<shared_ptr<Parameter>>, functionValue>> tba;

    void updateMap();

    void addValue(const pair<vector<shared_ptr<Parameter>>, functionValue> &val, set<functionValue *, PtrCmp> &set);

public:
    explicit ValueMap(unsigned int topEntries = 10);

    [[nodiscard]] functionValue query(const vector<shared_ptr<Parameter>> &params);

    void insert(const vector<shared_ptr<Parameter>> &params, functionValue val);

    [[nodiscard]] bool isKnown(const vector<shared_ptr<Parameter>> &cords);

    [[nodiscard]] functionValue getMedian();

    [[nodiscard]] unsigned long getSize() const;

    [[nodiscard]] list<pair<vector<shared_ptr<Parameter>>, functionValue>> getTopVals();
};


#endif //SIMOPTICON_VALUEMAP_H
