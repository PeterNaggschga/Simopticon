#include "Controller.h"

#include <memory>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>

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
    cout << "Initializing optimization...";
    cout.flush();

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
    double seconds = baseConfig.at("controller").at("updateInterval").get<double>();
    statusInterval = milliseconds((long) round(seconds * 1000));

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
        string omnetppDirectory = pipelineConfig.at("omnetppDirectory").get<string>();
        setenv("OMNETPP_HOME", omnetppDirectory.c_str(), true);

        pipelineId = baseConfig.at("pipeline").at("id").get<unsigned int>();

        unsigned int nrThreads = pipelineConfig.at("nrThreads").get<unsigned int>();
        filesystem::path scriptPath(pipelineConfig.at("pythonScript").get<string>());
        pipeline = unique_ptr<Pipeline>(new ConstantHeadway(nrThreads, scriptPath));
    } else {
        throw runtime_error("SimulationRunner not found: " + run);
    }

    cout << "done\n" << endl;
}

map<vector<shared_ptr<Parameter>>, functionValue>
Controller::requestValues(const list<vector<shared_ptr<Parameter>>> &params) {
    updateStatus();
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
        stepState.next();
        auto vecToResult = runSimulations(simRuns);
        stepState.next();
        auto simResults = evaluate(vecToResult);
        stepState.next();

        for (const auto &entry: simResults) {
            valueMap->insert(entry.first, entry.second);
            topResults.insert(make_pair(entry.first, vecToResult[entry.first].first));
        }

        removeOldResultfiles();

        result.merge(simResults);
    }
    updateStatus();
    return result;
}

ValueMap &Controller::getValueMap() {
    return *valueMap;
}

void Controller::run() {
    auto runStatusUpdate = [this]() {
        while (statusInterval != milliseconds(0)) {
            this_thread::sleep_for(statusInterval);
            updateStatus();
        }
    };
    stepState.next();
    updateStatus();
    thread statusThread(runStatusUpdate);
    optimizer->runOptimization();
    statusInterval = milliseconds(0);
    statusThread.join();
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
Controller::runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs) {
    updateStatus();
    auto res = runner->runSimulations(runs);
    updateStatus();
    return res;
}

map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> Controller::evaluate(
        const map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> &simulationResults) {
    updateStatus();
    set<pair<filesystem::path, set<runId>>> resultFiles;
    for (const auto &entry: simulationResults) {
        resultFiles.insert(entry.second);
    }
    auto evaluation = pipeline->processOutput(resultFiles, pipelineId);
    map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> result;
    for (const auto &entry: simulationResults) {
        result.insert(make_pair(entry.first, evaluation[entry.second]));
    }
    updateStatus();
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

void Controller::updateStatus() {
    pair<vector<shared_ptr<Parameter>>, functionValue> p =
            valueMap->getSize() != 0 ? valueMap->getTopVals().front() : make_pair(vector<shared_ptr<Parameter>>(),
                                                                                  (functionValue) INFINITY);
    bool stateChanged = stepState.stepChanged;
    statusBar.updateStatus(optimizer.get(), runner.get(), pipeline.get(), p, stateChanged, stepState.get());
}
