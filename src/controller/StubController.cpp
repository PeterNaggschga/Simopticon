#include "StubController.h"
#include "ValueMap.h"

#include <utility>

StubController::StubController(const list<shared_ptr<ParameterDefinition>> &def,
                               function<functionValue(vector<shared_ptr<Parameter>>)> f)
        : Controller(def), f(std::move(f)) {
}

void StubController::runSimulations(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) {
    result.clear();
    for (const auto &v: runs) {
        functionValue val = f(v);
        getValueMap().insert(v, val);
        result.insert(make_pair(v, val));
    }
}

map<vector<shared_ptr<Parameter>>, functionValue> StubController::evaluate() {
    return result;
}
