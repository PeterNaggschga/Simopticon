#include "ConstantHeadway.h"

#include <string>
#include <iostream>

using namespace std;

ConstantHeadway::ConstantHeadway(unsigned int threads) : MultithreadPipeline(threads), PythonScript(
        strcat(getenv("SIMOPTICON_HOME"), "/src/evaluation/constant_headway"),
        "constant_headway", "get_constant_headway") {
}

functionValue ConstantHeadway::processOutput(unsigned int pipelineId, filesystem::path path, set<runId> experimentIds) {
    functionValue returnVal;
    PyObject *pArgs, *pValue;
    pArgs = PyTuple_New(1);
    pValue = PyUnicode_FromString(path.c_str());
    if (!pValue) {
        Py_DECREF(pArgs);
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        throw invalid_argument("Python argument 'path' couldn't be created!");
    }
    PyTuple_SetItem(pArgs, 0, pValue);
    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    if (pValue) {
        returnVal = PyFloat_AsDouble(pValue);
        Py_DECREF(pValue);
    } else {
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        PyErr_Print();
        throw runtime_error("Call to Python function failed!");
    }
    return returnVal;
}

