#ifndef SIMOPTICON_VALUEMAP_H
#define SIMOPTICON_VALUEMAP_H


#include "../Types.h"

#include <map>
#include <vector>
#include <list>
#include <memory>

class Parameter;

using namespace std;

class ValueMap {
private:
    map<vector<shared_ptr<Parameter>>, functionValue> upperValues;
    map<vector<shared_ptr<Parameter>>, functionValue> lowerValues;
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
