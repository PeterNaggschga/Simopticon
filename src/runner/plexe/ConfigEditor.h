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
    const filesystem::path RESULTS;

    static void replaceOption(string &file, string option, const string &value, size_t start = 0);

    static void replaceOption(string &file, string option, integral auto value, size_t start = 0);

    void setResultFiles(string &file,
                        const map<vector<shared_ptr<Parameter>>, size_t, CmpVectorSharedParameter> &runToId,
                        unsigned int repeat);

    [[nodiscard]] static string getConfigAt(string &file, size_t start = 0);

public:
    explicit ConfigEditor(filesystem::path directory);

    size_t createConfig(map<vector<shared_ptr<Parameter>>, size_t, CmpVectorSharedParameter> runToId,
                        unsigned int repeat);

    void deleteConfig(size_t runId) const;

    static string getConfigValue(string &file, string option, size_t start = 0);

    [[nodiscard]] const filesystem::path &getDir() const;

    [[nodiscard]] filesystem::path getConfigPath(size_t runId) const;

    [[nodiscard]] filesystem::path getResultPath(size_t runId) const;

};


#endif //SIMOPTICON_CONFIGEDITOR_H
