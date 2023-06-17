#include "StubController.h"
#include "ValueMap.h"

#include <utility>
#include <cmath>

map<string, function<functionValue(vector<shared_ptr<Parameter>>)>> StubController::functions = {
        make_pair("branin", [](const vector<shared_ptr<Parameter>> &v) {
            functionValue res = pow(
                    v[1]->getVal() - (5.1 / (4 * pow(M_PI, 2))) * pow(v[0]->getVal(), 2) + (5 / M_PI) * v[0]->getVal() -
                    6,
                    2);
            return res + 10 * (1 - pow(8 * M_PI, -1)) * cos(v[0]->getVal()) + 10;
        }),
        make_pair("quadratic", [](const vector<shared_ptr<Parameter>> &v) {
            functionValue val = 0;
            for (const auto &par: v) {
                val += par->getVal() * par->getVal();
            }
            return val;
        })
};

StubController::StubController(const filesystem::path &configPath, const string &function) : f(functions[function]),
                                                                                             Controller(configPath) {
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
