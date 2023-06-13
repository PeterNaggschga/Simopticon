#ifndef SIMOPTICON_PYTHONSCRIPT_H
#define SIMOPTICON_PYTHONSCRIPT_H

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <filesystem>

#pragma clang diagnostic pop

class PythonScript {
protected:
    PyObject *pModule, *pFunc;

public:
    PythonScript(const std::filesystem::path &path, const char *functionName);

    ~PythonScript();

};


#endif //SIMOPTICON_PYTHONSCRIPT_H
