#include "ConfigEditor.h"

#include <fstream>
#include <utility>

ConfigEditor::ConfigEditor(filesystem::path directory, json controller)
        : DIR(directory), CONFIG(directory.append("omnetpp.ini")),
          RESULTS(directory.parent_path().append("optResults")), CONTROLLER(std::move(controller)) {
}

void ConfigEditor::createConfig(const vector<shared_ptr<Parameter>> &params, size_t runNumber, unsigned int repeat) {
    ifstream inStream(CONFIG);
    ostringstream textStream;
    textStream << inStream.rdbuf();
    string fileContents = textStream.str();
    inStream.close();

    setResultFiles(fileContents, runNumber);

    replaceOption(fileContents, "**.numericController", "${controller = 1}");
    replaceOption(fileContents, "**.headway", "${headway = " + to_string(CONTROLLER.at("headway")) + "! controller}s");
    replaceOption(fileContents, "**.traffic.controller",
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

string ConfigEditor::getConfigValue(string &file, string option, size_t start) {
    option = "\n" + option + " = ";
    size_t pos = file.find(option, start);
    if (pos == string::npos) {
        throw invalid_argument("Option not found in file!");
    }
    pos += option.size();
    size_t endOfLine = file.find('\n', pos);
    return file.substr(pos, endOfLine - pos);
}

void ConfigEditor::replaceOption(string &file, string option, const string &value, size_t start) {
    option = "\n" + option + " = ";
    size_t pos = file.find(option, start);
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

void ConfigEditor::replaceOption(string &file, string option, integral auto value, size_t start) {
    replaceOption(file, option, to_string(value), start);
}

void ConfigEditor::setResultFiles(string &file, size_t runNumber) {
    string resDir = "${resultdir}";
    size_t pos = 0;
    while ((pos = file.find(resDir, pos)) != string::npos) {
        file.replace(pos, resDir.size(), RESULTS.filename().string() + "/" + to_string(runNumber));
        pos++;
    }
}

filesystem::path ConfigEditor::getConfigPath(size_t runId) const {
    filesystem::path result = DIR;
    return result.append(".tmp" + to_string(runId) + ".ini");
}

filesystem::path ConfigEditor::getResultPath(size_t runId) const {
    filesystem::path result = RESULTS;
    return result.append(to_string(runId));
}

string ConfigEditor::getConfigAt(string &file, size_t start) {
    string conf = "\n[Config ";
    size_t pos = file.find(conf);
    string result;
    while (pos < start) {
        pos += conf.size();
        size_t end = file.find(']', pos);
        result = file.substr(pos, end - pos);
        pos = file.find(conf, end);
    }
    return result;
}

void ConfigEditor::deleteConfig(size_t runId) const {
    filesystem::remove(getConfigPath(runId));
}

const filesystem::path &ConfigEditor::getDir() const {
    return DIR;
}
