/**
 * @file
 * In this file the implementation of the PlexeSimulationRunner class is defined.
 */

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

bool
PlexeSimulationRunner::work(std::pair<std::filesystem::path, std::pair<std::basic_string<char>, unsigned int>> arg) {
    string command = "cd " + editor.getDir().string() + "; ";
    command += "./run -M release -s -u Cmdenv -c " + arg.second.first;
    command += " -r " + to_string(arg.second.second) + " " + arg.first.string();

    try {
        CommandLine::exec(command);
        return true;
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
    return false;

}

string PlexeSimulationRunner::getName() {
    return "Plexe-Runner";
}

string PlexeSimulationRunner::getStatus() {
    string status = "Evaluations:\t\t" + to_string(runNumber) + "\n";
    status += "Scenarios:\t\t" + SCENARIOS[0];
    for (int i = 1; i < SCENARIOS.size(); ++i) {
        status += ", " + SCENARIOS[i];
    }
    status += "\nRepeat:\t\t\t" + to_string(REPEAT) + "\n";
    status += "Max. number of threads:\t" + to_string(
            Multithreaded<pair<filesystem::path, pair<string, unsigned int>>, bool>::NR_THREADS *
            Multithreaded<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>::NR_THREADS);
    return status;
}

string PlexeSimulationRunner::getStatusBar() {
    auto runnerCurrent = Multithreaded<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>::queue.getSize();
    auto runnerStart = Multithreaded<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>::queue.getStartSize();
    auto plexeStart = Multithreaded<pair<filesystem::path, pair<string, unsigned int>>, bool>::queue.getStartSize();

    return "Running simulations... waiting: " + to_string(plexeStart * runnerCurrent) + ", running/done: " +
           to_string(plexeStart * (runnerStart - runnerCurrent));
}
