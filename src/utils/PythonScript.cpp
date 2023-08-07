/**
 * @file
 * In this file the implementation of the PythonScript class is defined.
 */

#include "PythonScript.h"

#include <stdexcept>
#include <filesystem>

using namespace std;

PythonScript::PythonScript(const filesystem::path &path, const char *functionName) {
    setenv("PYTHONPATH", path.parent_path().c_str(), 1);
    string scriptName = path.filename();
    scriptName.replace(scriptName.find(path.extension()), path.extension().string().size(), "");

    Py_Initialize();
    PyObject *pName = PyUnicode_DecodeFSDefault(scriptName.c_str());
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule) {
        pFunc = PyObject_GetAttrString(pModule, functionName);
        if (!(pFunc && PyCallable_Check(pFunc))) {
            if (PyErr_Occurred()) {
                PyErr_Print();
                throw runtime_error("Function couldn't be loaded!");
            }
        }
    } else {
        PyErr_Print();
        throw runtime_error("Python Module couldn't be loaded!");
    }
}

PythonScript::~PythonScript() {
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
}

