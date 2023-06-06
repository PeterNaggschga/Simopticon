#include "PlexeSimulationRunner.h"
#include "ConfigEditor.h"
#include "../../utils/CommandLine.h"

#include <utility>
#include <fstream>
#include <cmath>

PlexeSimulationRunner::PlexeSimulationRunner(unsigned int threads, unsigned int runs, unsigned int repeat,
                                             vector<string> scenarios, ConfigEditor editor) : SimulationRunner(threads,
                                                                                                               floor((long double) runs /
                                                                                                                     (repeat *
                                                                                                                      scenarios.size()))),
                                                                                              REPEAT(repeat), SCENARIOS(
                std::move(scenarios)), editor(std::move(editor)) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
PlexeSimulationRunner::runSimulationThread() {
    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> result;
    set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs;

    while (!(runs = getNextRun()).empty()) {
        map<vector<shared_ptr<Parameter>>, size_t, CmpVectorSharedParameter> runToId;
        for (const auto &entry: runs) {
            runToId.insert(make_pair(entry, getRunId()));
        }

        size_t iniNumber = editor.createConfig(runToId, REPEAT);

        string command = "cd " + editor.getDir().string();
        for (const auto &scenario: SCENARIOS) {
            string scenarioCmd = "; plexe_run -M release -s -u Cmdenv -c " + scenario;
            for (int i = 0; i < REPEAT * runs.size(); ++i) {
                command += scenarioCmd + " -r " + to_string(i) + " " + editor.getConfigPath(iniNumber).string();
            }
        }

        shadowResults(true);
        CommandLine::exec(command.c_str());
        shadowResults(false);
        editor.deleteConfig(iniNumber);

        for (const auto &entry: runToId) {
            set<runId> ids;
            filesystem::path resultDir = editor.getResultPath(entry.second);
            for (const auto &file: filesystem::directory_iterator(resultDir)) {
                if (file.path().extension().string() != ".vci") {
                    continue;
                }
                ifstream inStream(file.path());
                ostringstream textStream;
                textStream << inStream.rdbuf();
                string fileContents = textStream.str();
                inStream.close();

                size_t pos = fileContents.find("run ") + 4;
                size_t end = fileContents.find('\n', pos);
                ids.insert(fileContents.substr(pos, end - pos));
            }
            result.insert(make_pair(entry.first, make_pair(resultDir, ids)));
        }
    }
    return result;
}

size_t PlexeSimulationRunner::getRunId() {
    runNumberLock.lock();
    size_t result = runNumber;
    runNumber++;
    runNumberLock.unlock();
    return result;
}

void PlexeSimulationRunner::shadowResults(bool value) {
    resultsShadowLock.lock();
    resultsShadowed += value ? 1 : -1;
    filesystem::path origResults = editor.getDir(), shadowedResults = editor.getDir();
    origResults.append("results");
    shadowedResults.append(".results");
    if (value && resultsShadowed == 1) {
        filesystem::rename(origResults, shadowedResults);
    } else if (!value && resultsShadowed == 0) {
        filesystem::rename(shadowedResults, origResults);
    }
    resultsShadowLock.unlock();
}
