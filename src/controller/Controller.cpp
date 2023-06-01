#include "Controller.h"

#include "../parameters/Parameter.h"
#include "ValueMap.h"

map<vector<shared_ptr<Parameter>>, functionValue>
Controller::requestValues(const list<vector<shared_ptr<Parameter>>> &params) {
    map<vector<shared_ptr<Parameter>>, functionValue> result;
    list<vector<shared_ptr<Parameter>>> simRuns;
    for (const auto &cords: params) {
        if (valueMap->isKnown(cords)) {
            result.insert(make_pair(cords, valueMap->query(cords)));
        } else {
            simRuns.push_back(cords);
        }
    }

    // TODO: start Simulations in simRuns

    return result;
}