#ifndef SIMOPTICON_PYTHONSCRIPT_H
#define SIMOPTICON_PYTHONSCRIPT_H

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#pragma clang diagnostic pop

#include <vector>
#include <string>

using namespace std;

class PythonScript {
public:
    template<class RETURN>
    static RETURN
    executePythonFunction(string path, string scriptName, string functionName, vector<string> stringArgs = {},
                          vector<long long> longArgs = {}, vector<double> doubleArgs = {});

};

#include "PythonScript.tpp"

#endif //SIMOPTICON_PYTHONSCRIPT_H
