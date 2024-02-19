/**
 * @file
 * In this file, the implementation of the ConfigEditor class is defined.
 * @author Per Natzschka
 */

#include "ConfigEditor.h"

#include <fstream>
#include <utility>

ConfigEditor::ConfigEditor(std::filesystem::path directory, nlohmann::json controller)
        : DIR(directory), CONFIG(directory.append("omnetpp.ini")),
          RESULTS(directory.parent_path().append("optResults")), CONTROLLER(std::move(controller)) {
}

void ConfigEditor::createConfig(const parameterCombination &params, size_t runNumber, unsigned int repeat) {
    std::ifstream inStream(CONFIG);
    std::ostringstream textStream;
    textStream << inStream.rdbuf();
    std::string fileContents = textStream.str();
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
        replaceOption(fileContents, param->getConfig(), std::to_string(param->getVal()) + " " + param->getUnit());
    }

    std::ofstream outStream(getConfigPath(runNumber));
    outStream << fileContents;
    outStream.close();
}

void ConfigEditor::replaceOption(std::string &file, std::string option, const std::string &value) {
    option = "\n" + option + " = ";
    size_t pos = file.find(option);
    if (pos == std::string::npos) {
        const std::string general = "[General]\n";
        pos = file.find(general);
        file.insert(pos + general.size(), option + value + "\n");
        return;
    }
    pos += option.size();
    size_t endOfLine = file.find('\n', pos);
    file.replace(pos, endOfLine - pos, value);
}

void ConfigEditor::replaceOption(std::string &file, std::string option, long value) {
    replaceOption(file, std::move(option), std::to_string(value));
}

void ConfigEditor::setResultFiles(std::string &file, size_t runNumber) {
    const std::string resDir = "${resultdir}";
    size_t pos = 0;
    while ((pos = file.find(resDir, pos)) != std::string::npos) {
        file.replace(pos, resDir.size(), RESULTS.filename().string() + "/" + std::to_string(runNumber));
        pos++;
    }
}

std::string ConfigEditor::getControllerOption(std::string &file) {
    const std::string option = ".controller = ";
    size_t optionPos = file.find(option);
    size_t startOfLine = file.find('\n') + 1;
    size_t nextLine;
    while ((nextLine = file.find('\n', startOfLine)) < optionPos) {
        startOfLine = nextLine + 1;
    }
    return file.substr(startOfLine, optionPos - startOfLine + option.size() - 3);
}

std::filesystem::path ConfigEditor::getConfigPath(size_t runId) const {
    std::filesystem::path result = DIR;
    return result.append(".tmp" + std::to_string(runId) + ".ini");
}

std::filesystem::path ConfigEditor::getResultPath(size_t runId) const {
    std::filesystem::path result = RESULTS;
    return result.append(std::to_string(runId));
}

void ConfigEditor::deleteConfig(size_t runId) const {
    std::filesystem::remove(getConfigPath(runId));
}

const std::filesystem::path & ConfigEditor::getDir() const {
    return DIR;
}
