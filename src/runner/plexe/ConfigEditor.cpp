/**
 * @file
 * In this file, the implementation of the ConfigEditor class is defined.
 */

#include "ConfigEditor.h"

#include <fstream>
#include <utility>

ConfigEditor::ConfigEditor(filesystem::path directory, nlohmann::json controller)
        : DIR(directory), CONFIG(directory.append("omnetpp.ini")),
          RESULTS(directory.parent_path().append("optResults")), CONTROLLER(std::move(controller)) {
}

void ConfigEditor::createConfig(const parameterCombination &params, size_t runNumber, unsigned int repeat) {
    ifstream inStream(CONFIG);
    ostringstream textStream;
    textStream << inStream.rdbuf();
    string fileContents = textStream.str();
    inStream.close();

    setResultFiles(fileContents, runNumber);

    replaceOption(fileContents, "**.numericController", "${controller = 1}");
    replaceOption(fileContents, "**.headway", "${headway = " + to_string(CONTROLLER.at("headway")) + "! controller}s");
    replaceOption(fileContents, getControllerOption(fileContents),
                  "${sController = " + to_string(CONTROLLER.at("controller")) + " ! controller}");
    replaceOption(fileContents, "**.traffic.platoonInsertDistance", to_string(CONTROLLER.at("insertDistance")) + "m");
    replaceOption(fileContents, "**.traffic.platoonInsertHeadway", to_string(CONTROLLER.at("insertHeadway")) + "s");

    replaceOption(fileContents, "repeat", repeat);
    replaceOption(fileContents, "debug-on-errors", "false");
    replaceOption(fileContents, "print-undisposed", "false");
    replaceOption(fileContents, "cmdenv-autoflush", "false");
    replaceOption(fileContents, "cmdenv-status-frequency", "100s");

    for (auto &param: params) {
        replaceOption(fileContents, param->getConfig(), to_string(param->getVal()) + " " + param->getUnit());
    }

    ofstream outStream(getConfigPath(runNumber));
    outStream << fileContents;
    outStream.close();
}

void ConfigEditor::replaceOption(string &file, string option, const string &value) {
    option = "\n" + option + " = ";
    size_t pos = file.find(option);
    if (pos == string::npos) {
        const string general = "[General]\n";
        pos = file.find(general);
        file.insert(pos + general.size(), option + value + "\n");
        return;
    }
    pos += option.size();
    size_t endOfLine = file.find('\n', pos);
    file.replace(pos, endOfLine - pos, value);
}

void ConfigEditor::replaceOption(string &file, string option, long value) {
    replaceOption(file, std::move(option), to_string(value));
}

void ConfigEditor::setResultFiles(string &file, size_t runNumber) {
    const string resDir = "${resultdir}";
    size_t pos = 0;
    while ((pos = file.find(resDir, pos)) != string::npos) {
        file.replace(pos, resDir.size(), RESULTS.filename().string() + "/" + to_string(runNumber));
        pos++;
    }
}

string ConfigEditor::getControllerOption(string &file) {
    const string option = ".controller = ";
    size_t optionPos = file.find(option);
    size_t startOfLine = file.find('\n') + 1;
    size_t nextLine;
    while ((nextLine = file.find('\n', startOfLine)) < optionPos) {
        startOfLine = nextLine + 1;
    }
    return file.substr(startOfLine, optionPos - startOfLine + option.size() - 3);
}

filesystem::path ConfigEditor::getConfigPath(size_t runId) const {
    filesystem::path result = DIR;
    return result.append(".tmp" + to_string(runId) + ".ini");
}

filesystem::path ConfigEditor::getResultPath(size_t runId) const {
    filesystem::path result = RESULTS;
    return result.append(to_string(runId));
}

void ConfigEditor::deleteConfig(size_t runId) const {
    filesystem::remove(getConfigPath(runId));
}

const filesystem::path &ConfigEditor::getDir() const {
    return DIR;
}
