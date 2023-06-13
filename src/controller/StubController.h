#ifndef SIMOPTICON_STUBCONTROLLER_H
#define SIMOPTICON_STUBCONTROLLER_H


#include <functional>
#include "Controller.h"
#include "../optimizer/direct/DirectTypes.h"

class StubController : public Controller {
private:
    function<functionValue(vector<shared_ptr<Parameter>>)> f;

    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs) override;

    map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> evaluate(
            const map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> &simulationResults) override;

    void removeOldResultfiles() override;

public:
    explicit StubController(function<functionValue(vector<shared_ptr<Parameter>>)> f,
                            const filesystem::path &configPath); //TODO: config für Stubcontroller hinzufüen

};


#endif //SIMOPTICON_STUBCONTROLLER_H
