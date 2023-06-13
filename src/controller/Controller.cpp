#include "Controller.h"

#include <memory>
#include <utility>
#include <algorithm>
#include <fstream>

#include "ValueMap.h"
#include "../optimizer/direct/DirectOptimizer.h"
#include "../runner/plexe/PlexeSimulationRunner.h"
#include "../evaluation/constant_headway/ConstantHeadway.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

json getConfigByPath(filesystem::path baseDir, const string &config) {
    filesystem::path configPath = std::move(baseDir);
    configPath.append(config);
    ifstream configStream(configPath);
    json result = json::parse(configStream, nullptr, true, true);
    configStream.close();
    return result;
}

Controller::Controller(const filesystem::path &configPath) {
    json baseConfig = getConfigByPath(configPath.parent_path(), configPath.filename().string());
    json paramJson = getConfigByPath(configPath.parent_path(), baseConfig.at("controller").at("params").get<string>());
    json optimizerConfig = getConfigByPath(configPath.parent_path(),
                                           baseConfig.at("optimizer").at("config").get<string>());
    json runnerConfig = getConfigByPath(configPath.parent_path(), baseConfig.at("runner").at("config").get<string>());
    json pipelineConfig = getConfigByPath(configPath.parent_path(),
                                          baseConfig.at("pipeline").at("config").get<string>());

    // Controller settings
    keepFiles = baseConfig.at("controller").at("keepResultFiles").get<bool>();
    unsigned int nrTopEntries = baseConfig.at("controller").at("nrTopEntries").get<unsigned int>();
    valueMap = std::make_unique<ValueMap>(nrTopEntries);

    // Read Parameters
    list<shared_ptr<ParameterDefinition>> params;
    for (auto param: paramJson) {
        coordinate min = param.at("min").get<coordinate>();
        coordinate max = param.at("max").get<coordinate>();
        string config;
        if (param.contains("config")) {
            config = param.at("config").get<string>();
        }
        string unit;
        if (param.contains("unit")) {
            unit = param.at("unit").get<string>();
        }
        params.push_back(make_shared<ParameterDefinition>(min, max, config, unit));
    }

    // Optimizer settings
    string opt = baseConfig.at("optimizer").at("optimizer").get<string>();
    if (opt == "Direct") {
        optimizer = unique_ptr<Optimizer>(
                new DirectOptimizer(*this, params, StoppingCondition(optimizerConfig.at("stopCon"))));
    } else {
        throw runtime_error("Optimzer not found: " + opt);
    }

    // SimulationRunner settings
    string run = baseConfig.at("runner").at("runner").get<string>();
    if (run == "Plexe") {
        unsigned int nrThreads = runnerConfig.at("nrThreads").get<unsigned int>();
        unsigned int repeat = runnerConfig.at("repeat").get<unsigned int>();

        vector<string> scenarios(runnerConfig.at("scenarios").size());
        for (int i = 0; i < scenarios.size(); ++i) {
            scenarios[i] = runnerConfig.at("scenarios").at(i).get<string>();
        }

        string dir = runnerConfig.at("configDirectory").get<string>();
        runner = unique_ptr<SimulationRunner>(
                new PlexeSimulationRunner(nrThreads, repeat, scenarios, ConfigEditor(dir)));
    } else {
        throw runtime_error("SimulationRunner not found: " + run);
    }

    // Pipeline settings
    string pipe = baseConfig.at("pipeline").at("pipeline").get<string>();
    if (pipe == "ConstantHeadway") {
        pipelineId = baseConfig.at("pipeline").at("id").get<unsigned int>();
        unsigned int nrThreads = pipelineConfig.at("nrThreads").get<unsigned int>();

        pipeline = unique_ptr<Pipeline>(new ConstantHeadway(nrThreads));
    } else {
        throw runtime_error("SimulationRunner not found: " + run);
    }
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

        for (const auto &entry: simResults) {
            valueMap->insert(entry.first, entry.second);
            topResults.insert(make_pair(entry.first, vecToResult[entry.first].first));
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
