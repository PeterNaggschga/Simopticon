#include "StubController.h"
#include "ValueMap.h"

#include <utility>

StubController::StubController(const list<shared_ptr<ParameterDefinition>> &def,
                               function<functionValue(vector<shared_ptr<Parameter>>)> f)
        : Controller(def), f(std::move(f)) {
}

map<vector<shared_ptr<Parameter>>, functionValue>
StubController::requestValues(const list<vector<shared_ptr<Parameter>>> &params) {
    map<vector<shared_ptr<Parameter>>, functionValue> result;
    list<vector<shared_ptr<Parameter>>> simRuns;
    for (const auto &cords: params) {
        if (getValueMap().isKnown(cords)) {
            result.insert(make_pair(cords, getValueMap().query(cords)));
        } else {
            simRuns.push_back(cords);
        }
    }

    for (const auto &v: simRuns) {
        functionValue val = f(v);
        getValueMap().insert(v, val);
        result.insert(make_pair(v, val));
    }
    return result;
}
