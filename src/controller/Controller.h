#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H


#include "../Types.h"
#include "../ComparisonFunctions.h"
#include "../optimizer/Optimizer.h"
#include "../runner/SimulationRunner.h"
#include "../evaluation/Pipeline.h"
#include "../parameters/Parameter.h"

#include <map>
#include <vector>
#include <list>
#include <memory>
#include <set>

class ParameterDefinition;

class ValueMap;

using namespace std;

class Controller {
private:
    bool keepFiles;
    map<vector<shared_ptr<Parameter>>, filesystem::path> topResults;
    unsigned int pipelineId;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs);

    virtual map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> evaluate(
            const map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> &simulationResults);

    virtual void removeOldResultfiles();

protected:
    unique_ptr<Optimizer> optimizer;
    unique_ptr<SimulationRunner> runner;
    unique_ptr<Pipeline> pipeline;
    unique_ptr<ValueMap> valueMap;

public:
    Controller();

    void run();

    map<vector<shared_ptr<Parameter>>, functionValue> requestValues(const list<vector<shared_ptr<Parameter>>> &params);

    [[nodiscard]] ValueMap &getValueMap();
};


#endif //SIMOPTICON_CONTROLLER_H
