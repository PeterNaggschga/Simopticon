#include "PlexeSimulationRunner.h"
#include "ConfigEditor.h"
#include "../../utils/CommandLine.h"

#include <utility>
#include <fstream>
#include <iostream>

PlexeSimulationRunner::PlexeSimulationRunner(unsigned int threads, unsigned int repeat, vector<string> scenarios,
                                             ConfigEditor editor) : SimulationRunner(
        threads / min((size_t) threads, repeat * scenarios.size())),
                                                                    Multithreaded<pair<filesystem::path, pair<string, unsigned int>>, bool>(
                                                                            min((size_t) threads,
                                                                                repeat * scenarios.size())),
                                                                    REPEAT(repeat), SCENARIOS(std::move(scenarios)),
                                                                    editor(std::move(editor)) {
}

pair<filesystem::path, set<runId>> PlexeSimulationRunner::work(vector<shared_ptr<Parameter>> run) {
    size_t iniNumber = getRunId();
    editor.createConfig(run, iniNumber, REPEAT);

    string configPath = editor.getConfigPath(iniNumber);
    set<pair<filesystem::path, pair<string, unsigned int>>> runs;
    for (const auto &scenario: SCENARIOS) {
        for (int i = 0; i < REPEAT; ++i) {
            runs.insert(make_pair(configPath, make_pair(scenario, i)));
        }
    }
    Multithreaded<pair<filesystem::path, pair<string, unsigned int>>, bool>::runMultithreadedFunctions(runs);

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
    }
    return make_pair(resultDir, ids);
}

size_t PlexeSimulationRunner::getRunId() {
    runNumberLock.lock();
    size_t result = runNumber;
    runNumber++;
    runNumberLock.unlock();
    return result;
}

bool PlexeSimulationRunner::work(pair<filesystem::path, pair<string, unsigned int>> arg) {
    string command = "cd " + editor.getDir().string() + "; ";
    command += "plexe_run -M release -s -u Cmdenv -c " + arg.second.first;
    command += " -r " + to_string(arg.second.second) + " " + arg.first.string();

    try {
        CommandLine::exec(command.c_str());
        return true;
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
    return false;

}
