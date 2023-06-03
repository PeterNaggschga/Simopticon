#include "Controller.h"

#include "../parameters/Parameter.h"
#include "ValueMap.h"
#include "../optimizer/direct/DirectOptimizer.h"
#include "../runner/PlexeSimulationRunner.h"
#include "../evaluation/ConstantHeadway.h"

Controller::Controller(const list<shared_ptr<ParameterDefinition>> &params) : valueMap(new ValueMap()) {
    //TODO: Optimizer aus config lesen
    auto con = StoppingCondition(0, 50000); // TODO: aus config lesen
    //TODO: params aus config lesen
    Controller::optimizer = unique_ptr<Optimizer>(new DirectOptimizer(*this, params, params.size(), con));
    //TODO: runner aus config lesen
    Controller::runner = unique_ptr<SimulationRunner>(new PlexeSimulationRunner());
    //TODO: pipeline aus config lesen
    Controller::pipeline = unique_ptr<Pipeline>(new ConstantHeadway());
}

map<vector<shared_ptr<Parameter>>, functionValue>
Controller::requestValues(const list<vector<shared_ptr<Parameter>>> &params) {
    map<vector<shared_ptr<Parameter>>, functionValue> result;
    set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> simRuns;
    for (const auto &cords: params) {
        if (getValueMap().isKnown(cords)) {
            result.insert(make_pair(cords, getValueMap().query(cords)));
        } else {
            simRuns.insert(cords);
        }
    }

    runSimulations(simRuns);

    auto simResults = evaluate();
    result.insert(simResults.begin(), simResults.end());

    return result;
}

ValueMap &Controller::getValueMap() {
    return *valueMap;
}

void Controller::run() {
    optimizer->runOptimization();
}

void
Controller::runSimulations(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) {
}

map<vector<shared_ptr<Parameter>>, functionValue> Controller::evaluate() {
    return {};
}
