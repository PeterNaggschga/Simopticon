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
    filesystem::path CONFIG;
    filesystem::path RESULTS;

    static void replaceOption(string &file, string option, const string &value, size_t start = 0);

    static void replaceOption(string &file, string option, integral auto value, size_t start = 0);

    static void setResultFiles(string &file,
                               const map<vector<shared_ptr<Parameter>>, unsigned long, CmpVectorSharedParameter> &runToId,
                               unsigned int repeat);

    [[nodiscard]] static string getConfigAt(string &file, size_t start = 0);

public:
    explicit ConfigEditor(const filesystem::path &directory);

    unsigned long createConfig(map<vector<shared_ptr<Parameter>>, unsigned long, CmpVectorSharedParameter> runToId,
                               unsigned int repeat);

    void deleteConfig(unsigned long runId) const;

    static string getConfigValue(string &file, string option, size_t start = 0);


    [[nodiscard]] filesystem::path getConfigPath(unsigned long runId) const;

    [[nodiscard]] filesystem::path getResultPath(unsigned long runId) const;

};


#endif //SIMOPTICON_CONFIGEDITOR_H
