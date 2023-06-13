#include "StubController.h"
#include "ValueMap.h"

#include <utility>

StubController::StubController(function<functionValue(vector<shared_ptr<Parameter>>)> f,
                               const filesystem::path &configPath) : f(std::move(f)), Controller(configPath) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
StubController::runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs) {
    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> result;
    for (const auto &v: runs) {
        result.insert(make_pair(v, make_pair("", set<runId>())));
    }
    return result;
}

map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> StubController::evaluate(
        const map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> &simulationResults) {
    map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> result;
    for (const auto &v: simulationResults) {
        result.insert(make_pair(v.first, f(v.first)));
    }
    return result;
}

void StubController::removeOldResultfiles() {
}
