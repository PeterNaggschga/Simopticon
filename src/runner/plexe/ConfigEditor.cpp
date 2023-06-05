#include "ConfigEditor.h"

#include <utility>
#include <fstream>
#include <list>

ConfigEditor::ConfigEditor(filesystem::path directory) : DIR(directory), CONFIG(directory.append("omnetpp.ini")),
                                                         RESULTS(directory.parent_path().append("optResults")) {
}

size_t
ConfigEditor::createConfig(map<vector<shared_ptr<Parameter>>, size_t, CmpVectorSharedParameter> runToId,
                           unsigned int repeat) {
    size_t iniNumber = runToId.begin()->second;

    ifstream inStream(CONFIG);
    ostringstream textStream;
    textStream << inStream.rdbuf();
    string fileContents = textStream.str();
    inStream.close();

    setResultFiles(fileContents, runToId, repeat);

    replaceOption(fileContents, "repeat", repeat * runToId.size());
    replaceOption(fileContents, "debug-on-errors", "false");
    replaceOption(fileContents, "print-undisposed", "false");
    replaceOption(fileContents, "cmdenv-autoflush", "false");
    replaceOption(fileContents, "cmdenv-status-frequency", "100s");

    vector<string> paramStrings(runToId.begin()->first.size(), "${");
    for (const auto &entry: runToId) {
        for (int i = 0; i < paramStrings.size(); ++i) {
            coordinate val = entry.first[i]->getVal();
            for (int j = 0; j < repeat; ++j) {
                paramStrings[i] += to_string(val) + ", ";
            }
        }
    }

    auto entry = runToId.begin()->first;
    for (int i = 0; i < paramStrings.size(); ++i) {
        paramStrings[i] += "! repetition}" + entry[i]->getUnit();
        replaceOption(fileContents, entry[i]->getConfig(), paramStrings[i]);
    }

    ofstream outStream(getConfigPath(iniNumber));
    outStream << fileContents;
    outStream.close();

    return iniNumber;
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
        throw invalid_argument("Option not found in file!");
    }
    pos += option.size();
    size_t endOfLine = file.find('\n', pos);
    file.replace(pos, endOfLine - pos, value);
}

void ConfigEditor::replaceOption(string &file, string option, integral auto value, size_t start) {
    replaceOption(file, option, to_string(value), start);
}

void ConfigEditor::setResultFiles(string &file,
                                  const map<vector<shared_ptr<Parameter>>, size_t, CmpVectorSharedParameter> &runToId,
                                  unsigned int repeat) {
    vector<size_t> runIds;
    runIds.reserve(runToId.size());
    for (const auto &entry: runToId) {
        runIds.push_back(entry.second);
    }

    size_t pos = file.find("\noutput-");
    while (pos != string::npos) {
        size_t end = file.find(" = ", pos);
        string option = file.substr(pos + 1, end - pos - 1);
        string resultDirectory = getConfigValue(file, option, pos);

        string newVal = "${";
        for (auto id: runIds) {
            string outDir = resultDirectory;
            string resDir = "${resultdir}";
            size_t searchPos = outDir.find(resDir);
            if (searchPos != string::npos) {
                string results = RESULTS;
                outDir.replace(searchPos, resDir.size(), results + "/" + to_string(id));
            }
            string confName = "${configname}";
            searchPos = outDir.find(confName);
            if (searchPos != string::npos) {
                outDir.replace(searchPos, confName.size(), getConfigAt(file, pos));
            }
            for (int j = 0; j < repeat; ++j) {
                newVal += outDir + ", ";
            }
        }
        resultDirectory = newVal + "! repetition}";

        replaceOption(file, option, resultDirectory, pos);
        pos = file.find("\noutput-", ++pos);
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
