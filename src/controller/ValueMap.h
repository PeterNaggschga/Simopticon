#ifndef SIMOPTICON_VALUEMAP_H
#define SIMOPTICON_VALUEMAP_H


#include "../Types.h"
#include "../ComparisonFunctions.h"
#include "../parameters/Parameter.h"

#include <map>
#include <vector>
#include <list>
#include <memory>
#include <set>

using namespace std;

class ValueMap {
private:
    set<functionValue *, CmpPtrFunctionvalue> upperValues;
    set<functionValue *, CmpPtrFunctionvalue> lowerValues;

    unsigned int topEntries;
    set<pair<const vector<shared_ptr<Parameter>>, functionValue>, CmpPairVectorSharedParameterFunctionvalue> topVals;

    map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> values;
    list<pair<vector<shared_ptr<Parameter>>, functionValue>> tba;

    void updateMap();

    void addValue(const pair<vector<shared_ptr<Parameter>>, functionValue> &val,
                  set<functionValue *, CmpPtrFunctionvalue> &set);

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
