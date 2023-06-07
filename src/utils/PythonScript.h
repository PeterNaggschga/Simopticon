#ifndef SIMOPTICON_PYTHONSCRIPT_H
#define SIMOPTICON_PYTHONSCRIPT_H

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#pragma clang diagnostic pop

class PythonScript {
protected:
    PyObject *pModule, *pFunc;

public:
    explicit PythonScript(const char *path, const char *scriptName);

    ~PythonScript();

};


#endif //SIMOPTICON_PYTHONSCRIPT_H
