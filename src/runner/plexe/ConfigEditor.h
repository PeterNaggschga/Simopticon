#ifndef SIMOPTICON_CONFIGEDITOR_H
#define SIMOPTICON_CONFIGEDITOR_H


#include "../../ComparisonFunctions.h"

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

class Parameter;

using namespace std;

class ConfigEditor {
private:
    const filesystem::path DIR;
    const filesystem::path CONFIG;

    static void replaceOption(string &file, string option, const string &value);

    static void replaceOption(string &file, string option, integral auto value);

    static void replaceOptionValue(string &file, string option, const string &before, const string &after);

public:
    explicit ConfigEditor(filesystem::path directory);

    unsigned long createConfig(map<vector<shared_ptr<Parameter>>, unsigned long, CmpVectorSharedParameter> runToId,
                               unsigned int repeat);

    [[nodiscard]] filesystem::path getConfigPath(unsigned long runId) const;

    [[nodiscard]] filesystem::path getResultPath(unsigned long runId) const;

};


#endif //SIMOPTICON_CONFIGEDITOR_H
