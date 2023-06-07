#include "PythonScript.h"

#include <stdexcept>

using namespace std;

PythonScript::PythonScript(const char *path, const char *scriptName, const char *functionName) {
    setenv("PYTHONPATH", path, 1);

    Py_Initialize();
    PyObject *pName = PyUnicode_DecodeFSDefault(scriptName);
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
    Py_Finalize();
}

