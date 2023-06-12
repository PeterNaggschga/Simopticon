#include "Controller.h"

#include <utility>

#include "ValueMap.h"
#include "../optimizer/direct/DirectOptimizer.h"
#include "../runner/plexe/PlexeSimulationRunner.h"
#include "../evaluation/constant_headway/ConstantHeadway.h"

Controller::Controller() : valueMap(new ValueMap()) {
    //TODO: Optimizer aus config lesen
    auto con = StoppingCondition(100); // TODO: aus config lesen
    shared_ptr<ParameterDefinition> c1(new ParameterDefinition(0, 1, "*.node[*].scenario.caccC1"));
    shared_ptr<ParameterDefinition> omegaN(new ParameterDefinition(0.05, 15, "*.node[*].scenario.caccOmegaN", "Hz"));
    shared_ptr<ParameterDefinition> xi(new ParameterDefinition(1, 5, "*.node[*].scenario.caccXi"));
    shared_ptr<ParameterDefinition> spacing(new ParameterDefinition(5, 7, "*.node[*].scenario.caccSpacing", "m"));
    list<shared_ptr<ParameterDefinition>> params({c1, xi, omegaN, spacing});
    //TODO: params aus config lesen
    Controller::optimizer = unique_ptr<Optimizer>(new DirectOptimizer(*this, params, con));
    //TODO: runner aus config lesen
    keepFiles = true; //TODO: aus config lesen
    ConfigEditor edit = ConfigEditor("/home/petern/src/plexe/examples/platooning"); //TODO: aus config lesen
    Controller::runner = unique_ptr<SimulationRunner>(
            new PlexeSimulationRunner(16, 3, {"SinusoidalNoGui", "BrakingNoGui"}, edit));
    //TODO: pipeline aus config lesen
    Controller::pipeline = unique_ptr<Pipeline>(new ConstantHeadway(16));
    pipelineId = 0; //TODO: aus config lesen
}

map<vector<shared_ptr<Parameter>>, functionValue>
Controller::requestValues(const list<vector<shared_ptr<Parameter>>> &params) {
    map<vector<shared_ptr<Parameter>>, functionValue> result;
    set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> simRuns;
    for (const auto &cords: params) {
        if (valueMap->isKnown(cords)) {
            result.insert(make_pair(cords, valueMap->query(cords)));
        } else {
            simRuns.insert(cords);
        }
    }

    if (!simRuns.empty()) {
        auto vecToResult = runSimulations(simRuns);
        auto simResults = evaluate(vecToResult);

        functionValue leastVal = valueMap->getTopVals().rbegin()->second;
        for (const auto &entry: simResults) {
            if (entry.second < leastVal) {
                leastVal = entry.second;
                topResults.insert(make_pair(entry.first, vecToResult[entry.first].first));
            } else {
                filesystem::remove_all(vecToResult[entry.first].first);
            }
            valueMap->insert(entry.first, entry.second);
        }

        removeOldResultfiles();

        result.merge(simResults);
    }

    return result;
}

ValueMap &Controller::getValueMap() {
    return *valueMap;
}

void Controller::run() {
    optimizer->runOptimization();
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
Controller::runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs) {
    return runner->runSimulations(runs);
}

map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> Controller::evaluate(
        const map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> &simulationResults) {
    set<pair<filesystem::path, set<runId>>> resultFiles;
    for (const auto &entry: simulationResults) {
        resultFiles.insert(entry.second);
    }
    auto evaluation = pipeline->processOutput(resultFiles, pipelineId);
    map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> result;
    for (const auto &entry: simulationResults) {
        result.insert(make_pair(entry.first, evaluation[entry.second]));
    }
    return result;
}

void Controller::removeOldResultfiles() {
    if (!(keepFiles || topResults.empty())) {
        filesystem::path resultDir = topResults.begin()->second.parent_path();
        filesystem::remove_all(resultDir);
    } else if (topResults.size() > valueMap->getTopVals().size()) {
        list<vector<shared_ptr<Parameter>>> toBeRemoved;
        for (const auto &entry: topResults) {
            if (!valueMap->isTopValue(entry.first)) {
                filesystem::remove_all(entry.second);
                toBeRemoved.push_back(entry.first);
            }
        }
        for (const auto &entry: toBeRemoved) {
            topResults.erase(entry);
        }
    }
}
