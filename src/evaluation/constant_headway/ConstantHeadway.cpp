#include "ConstantHeadway.h"

#include "../../utils/CommandLine.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#pragma clang diagnostic pop

#include <string>
#include <iostream>

using namespace std;

ConstantHeadway::ConstantHeadway() {
    // TODO: bei Installation $SIMOPTICON_HOME setzen
    string scriptPath = getenv("SIMOPTICON_HOME");
    scriptPath += "/src/evaluation/constant_headway";
    setenv("PYTHONPATH", scriptPath.c_str(), 1);
}

void ConstantHeadway::processOutput(set<runId> experimentIds, filesystem::path path, unsigned int pipelineId) {
    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;

    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault("constant_headway");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule) {
        pFunc = PyObject_GetAttrString(pModule, "getConstantHeadway");

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(1);
            pValue = PyUnicode_FromString(path.c_str());
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                throw invalid_argument("Python argument 'path' couldn't be created!");
            }
            PyTuple_SetItem(pArgs, 0, pValue);
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue) {
                value = PyFloat_AsDouble(pValue);
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                throw runtime_error("Call to Python function failed!");
            }
        } else {
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
    }
    if (Py_FinalizeEx() < 0) {
        throw runtime_error("Python finalization failed!");
    }
}

functionValue ConstantHeadway::getValue() {
    return value;
}
