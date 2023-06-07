#include "PlexeSimulationRunner.h"
#include "ConfigEditor.h"
#include "../../utils/CommandLine.h"

#include <utility>
#include <fstream>

PlexeSimulationRunner::PlexeSimulationRunner(unsigned int threads, unsigned int repeat, vector<string> scenarios,
                                             ConfigEditor editor) : SimulationRunner(threads),
                                                                    REPEAT(repeat), SCENARIOS(
                std::move(scenarios)), editor(std::move(editor)) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
PlexeSimulationRunner::runSimulationThread() {
    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> result;
    vector<shared_ptr<Parameter>> run;

    while (!(run = getNextRun()).empty()) {
        size_t iniNumber = getRunId();
        editor.createConfig(run, iniNumber, REPEAT);

        string command = "cd " + editor.getDir().string();
        for (const auto &scenario: SCENARIOS) {
            string scenarioCmd = "; plexe_run -M release -s -u Cmdenv -c " + scenario;
            for (int i = 0; i < REPEAT; ++i) {
                command += scenarioCmd + " -r " + to_string(i) + " " + editor.getConfigPath(iniNumber).string();
            }
        }

        CommandLine::exec(command.c_str());

        editor.deleteConfig(iniNumber);

        set<runId> ids;
        filesystem::path resultDir = editor.getResultPath(iniNumber);
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
            result.insert(make_pair(run, make_pair(resultDir, ids)));
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

