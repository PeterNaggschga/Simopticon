#include "Controller.h"

#include "../parameters/Parameter.h"
#include "ValueMap.h"
#include "../optimizer/Optimizer.h"
#include "../evaluation/Pipeline.h"
#include "../optimizer/direct/DirectOptimizer.h"
#include "../runner/PlexeSimulationRunner.h"
#include "../evaluation/ConstantHeadway.h"

Controller::Controller(const list<shared_ptr<ParameterDefinition>> &params) : valueMap(new ValueMap()) {
    //TODO: Optimizer aus config lesen
    StoppingCondition con = StoppingCondition(10, 15); // TODO: aus config lesen
    //TODO: params aus config lesen
    Controller::optimizer = unique_ptr<Optimizer>(new DirectOptimizer(*this, params, params.size(), con));
    //TODO: runner aus config lesen
    Controller::runner = unique_ptr<SimulationRunner>(new PlexeSimulationRunner());
    //TODO: pipeline aus config lesen
    Controller::pipeline = unique_ptr<Pipeline>(new ConstantHeadway());
    Controller::optimizer->runOptimization();
}

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

    // TODO: evaluate results

    return result;
}

ValueMap &Controller::getValueMap() {
    return *valueMap;
}
