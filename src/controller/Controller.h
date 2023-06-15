#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H


#include "../Types.h"
#include "../ComparisonFunctions.h"
#include "../optimizer/Optimizer.h"
#include "../runner/SimulationRunner.h"
#include "../evaluation/Pipeline.h"
#include "../parameters/Parameter.h"
#include "../status/StatusBar.h"
#include "../utils/Abortable.h"

#include <map>
#include <vector>
#include <list>
#include <memory>
#include <set>

class ParameterDefinition;

class ValueMap;

using namespace std;

class Controller : public Abortable {
private:
    bool keepFiles;
    map<vector<shared_ptr<Parameter>>, filesystem::path> topResults;
    unsigned int pipelineId;

    struct stepstate {
        bool stepChanged;
        step currentStep = INIT;

        void next() {
            stepChanged = true;
            currentStep = static_cast<step>((currentStep + 1) % 3);
        }

        step get() {
            stepChanged = false;
            return currentStep;
        }
    } stepState;

    chrono::milliseconds statusInterval = chrono::milliseconds(0);

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs);

    virtual map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> evaluate(
            const map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> &simulationResults);

    virtual void removeOldResultfiles();

    virtual void updateStatus();

protected:
    StatusBar statusBar;
    unique_ptr<Optimizer> optimizer;
    unique_ptr<SimulationRunner> runner;
    unique_ptr<Pipeline> pipeline;
    unique_ptr<ValueMap> valueMap;

public:
    explicit Controller(const filesystem::path &configPath);

    void run();

    map<vector<shared_ptr<Parameter>>, functionValue> requestValues(const list<vector<shared_ptr<Parameter>>> &params);

    [[nodiscard]] ValueMap &getValueMap();

    void abort() override;
};


#endif //SIMOPTICON_CONTROLLER_H
