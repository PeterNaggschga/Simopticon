#ifndef SIMOPTICON_STUBCONTROLLER_H
#define SIMOPTICON_STUBCONTROLLER_H


#include <functional>
#include "Controller.h"
#include "../optimizer/direct/DirectTypes.h"

class StubController : public Controller {
private:
    function<functionValue(vector<shared_ptr<Parameter>>)> f;
    map<vector<shared_ptr<Parameter>>, functionValue> result;

public:
    explicit StubController(const list<shared_ptr<ParameterDefinition>> &def,
                            function<functionValue(vector<shared_ptr<Parameter>>)> f);

    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs) override;

    map<vector<shared_ptr<Parameter>>, functionValue> evaluate() override;
};


#endif //SIMOPTICON_STUBCONTROLLER_H
