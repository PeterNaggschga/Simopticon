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
        bool operator()(const functionValue *a, const functionValue *b) const { return *a < *b; }
    };

    set<functionValue *, PtrCmp> upperValues;
    set<functionValue *, PtrCmp> lowerValues;
    map<vector<shared_ptr<Parameter>>, functionValue> values;
    list<pair<vector<shared_ptr<Parameter>>, functionValue>> tba;

    void updateMap();

public:
    ValueMap() = default;

    [[nodiscard]] functionValue query(const vector<shared_ptr<Parameter>> &params);

    void insert(const vector<shared_ptr<Parameter>> &params, functionValue val);

    [[nodiscard]] bool isKnown(const vector<shared_ptr<Parameter>> &cords);

    [[nodiscard]] functionValue getMedian();

    [[nodiscard]] unsigned long getSize() const;
};


#endif //SIMOPTICON_VALUEMAP_H
