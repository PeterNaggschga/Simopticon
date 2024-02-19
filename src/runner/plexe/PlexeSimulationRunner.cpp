/**
 * @file
 * In this file, the implementation of the PlexeSimulationRunner class is defined.
 * @author Per Natzschka
 */

#include "PlexeSimulationRunner.h"
#include "ConfigEditor.h"
#include "../../utils/CommandLine.h"

#include <utility>
#include <fstream>
#include <iostream>

PlexeSimulationRunner::PlexeSimulationRunner(unsigned int threads, unsigned int repeat,
                                             std::vector<std::string> scenarios, ConfigEditor editor) :
        SimulationRunner(threads / std::min((size_t) threads, repeat * scenarios.size())),
        Multithreaded<std::pair<std::filesystem::path, std::pair<std::string, unsigned int>>, bool>(
                std::min((size_t) threads,
                         repeat * scenarios.size())), REPEAT(repeat), SCENARIOS(std::move(scenarios)),
        editor(std::move(editor)) {
}

std::pair<std::filesystem::path, std::set<runId>> PlexeSimulationRunner::work(parameterCombination run) {
    size_t iniNumber = getRunId();
    editor.createConfig(run, iniNumber, REPEAT);

    std::string configPath = editor.getConfigPath(iniNumber);
    std::set<std::pair<std::filesystem::path, std::pair<std::string, unsigned int>>> runs;
    for (const auto &scenario: SCENARIOS) {
        for (int i = 0; i < REPEAT; ++i) {
            runs.insert(make_pair(configPath, make_pair(scenario, i)));
        }
    }
    Multithreaded<std::pair<std::filesystem::path, std::pair<std::string, unsigned int>>, bool>::runMultithreadedFunctions(
            runs);

    editor.deleteConfig(iniNumber);

    std::set<runId> ids;
    std::filesystem::path resultDir = editor.getResultPath(iniNumber);
    for (const auto &file: std::filesystem::directory_iterator(resultDir)) {
        if (file.path().extension().string() != ".vci") {
            continue;
        }
        std::ifstream inStream(file.path());
        std::ostringstream textStream;
        textStream << inStream.rdbuf();
        std::string fileContents = textStream.str();
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
    std::string command = "cd " + editor.getDir().string() + "; ";
    command += "./run -M release -s -u Cmdenv -c " + arg.second.first;
    command += " -r " + std::to_string(arg.second.second) + " " + arg.first.string();

    try {
        CommandLine::exec(command);
        return true;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return false;

}

std::string PlexeSimulationRunner::getName() {
    return "Plexe-Runner";
}

std::string PlexeSimulationRunner::getStatus() {
    std::string status = "Evaluations:\t\t" + std::to_string(runNumber) + "\n";
    status += "Scenarios:\t\t" + SCENARIOS[0];
    for (int i = 1; i < SCENARIOS.size(); ++i) {
        status += ", " + SCENARIOS[i];
    }
    status += "\nRepeat:\t\t\t" + std::to_string(REPEAT) + "\n";
    status += "Max. number of threads:\t" + std::to_string(
            Multithreaded<std::pair<std::filesystem::path, std::pair<std::string, unsigned int>>, bool>::NR_THREADS *
            Multithreaded<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>::NR_THREADS);
    return status;
}

std::string PlexeSimulationRunner::getStatusBar() {
    auto runnerCurrent = Multithreaded<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>::queue.getSize();
    auto runnerStart = Multithreaded<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>::queue.getStartSize();
    auto plexeStart = Multithreaded<std::pair<std::filesystem::path, std::pair<std::string, unsigned int>>, bool>::queue.getStartSize();

    return "Running simulations... waiting: " + std::to_string(plexeStart * runnerCurrent) + ", running/done: " +
           std::to_string(plexeStart * (runnerStart - runnerCurrent));
}
