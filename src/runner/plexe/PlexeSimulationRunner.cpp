#include "PlexeSimulationRunner.h"
#include "ConfigEditor.h"

#include <utility>

PlexeSimulationRunner::PlexeSimulationRunner(unsigned int threads, unsigned int runs, unsigned int repeat,
                                             vector<string> scenarios, ConfigEditor editor) : SimulationRunner(threads,
                                                                                                               runs /
                                                                                                               (repeat *
                                                                                                                scenarios.size()) +
                                                                                                               1),
                                                                                              REPEAT(repeat), SCENARIOS(
                std::move(scenarios)), editor(std::move(editor)) {
}

map<vector<shared_ptr<Parameter>>, runId, CmpVectorSharedParameter>
PlexeSimulationRunner::runSimulationThread(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) {
    map<vector<shared_ptr<Parameter>>, size_t, CmpVectorSharedParameter> runToId;
    for (const auto &entry: runs) {
        runToId.insert(make_pair(entry, getRunId()));
    }

    size_t iniNumber = editor.createConfig(runToId, REPEAT);

    string command = "cd " + editor.getDir().string();

    for (const auto &scenario: SCENARIOS) {
        string scenarioCmd = " && plexe_run -u Cmdenv -c " + scenario;
        for (int i = 0; i < REPEAT * runs.size(); ++i) {
            command += scenarioCmd + " -r " + to_string(i) + " " + editor.getConfigPath(iniNumber).string();
        }
    }

    editor.deleteConfig(iniNumber);

    return {};
}

size_t PlexeSimulationRunner::getRunId() {
    runNumberLock.lock();
    size_t result = runNumber;
    runNumber++;
    runNumberLock.unlock();
    return result;
}
