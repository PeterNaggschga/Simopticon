#include "ConfigEditor.h"

#include <utility>
#include <fstream>

ConfigEditor::ConfigEditor(filesystem::path directory) : DIR(directory), CONFIG(directory.append("omnetpp.ini")) {
}

unsigned long
ConfigEditor::createConfig(map<vector<shared_ptr<Parameter>>, unsigned long, CmpVectorSharedParameter> runToId,
                           unsigned int repeat) {
    unsigned long iniNumber = runToId.begin()->second;

    ifstream inStream(CONFIG);
    ostringstream textStream;
    textStream << inStream.rdbuf();
    string fileContents = textStream.str();
    inStream.close();

    replaceOptionValue(fileContents, "output-scalar-file", "${resultdir}", "${resultdir}/" + to_string(iniNumber));
    replaceOptionValue(fileContents, "output-vector-file", "${resultdir}", "${resultdir}/" + to_string(iniNumber));

    replaceOption(fileContents, "repeat", repeat * runToId.size());

    // TODO: parameter setzen
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

void ConfigEditor::replaceOption(string &file, string option, const string &value) {
    option = "\n" + option + " = ";
    size_t pos = file.find(option);
    while (pos != string::npos) {
        pos += option.size();
        size_t endOfLine = file.find('\n', pos);
        file.replace(pos, endOfLine - pos, value);
        pos = file.find(option, pos + option.size());
    }
}

void ConfigEditor::replaceOption(string &file, string option, integral auto value) {
    replaceOption(file, option, to_string(value));
}

void ConfigEditor::replaceOptionValue(string &file, string option, const string &before, const string &after) {
    option = "\n" + option + " = ";
    size_t pos = file.find(option);
    while (pos != string::npos) {
        pos += option.size();
        size_t endOfLine = file.find('\n', pos);
        pos = file.find(before, pos);
        if (endOfLine < pos) {
            return;
        }
        file.replace(pos, before.size(), after);
        pos = file.find(option, pos);
    }
}

filesystem::path ConfigEditor::getConfigPath(unsigned long runId) const {
    filesystem::path result = DIR;
    return result.append(".tmp" + to_string(runId) + ".ini");
}

filesystem::path ConfigEditor::getResultPath(unsigned long runId) const {
    filesystem::path result = DIR;
    return result.append("results").append(to_string(runId));
}
