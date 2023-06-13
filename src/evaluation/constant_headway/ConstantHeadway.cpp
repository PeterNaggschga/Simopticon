#include "ConstantHeadway.h"


using namespace std;

ConstantHeadway::ConstantHeadway(unsigned int nrThreads, const filesystem::path &pathToScript) : PythonScript(
        pathToScript.c_str(), "constant_headway", "multithreaded"), NR_THREADS(nrThreads) {
}

map<pair<filesystem::path, set<runId>>, functionValue>
ConstantHeadway::processOutput(const set<pair<filesystem::path, set<runId>>> &experimentResults,
                               unsigned int pipelineId) {
    map<pair<filesystem::path, set<runId>>, functionValue> result;

    PyObject *pArgs, *pValue, *pList;
    pArgs = PyTuple_New(3);
    PyTuple_SetItem(pArgs, 0, secureValue(PyLong_FromLong(NR_THREADS)));
    PyTuple_SetItem(pArgs, 1,
                    secureValue(PyUnicode_FromString(experimentResults.begin()->first.parent_path().c_str())));

    pValue = secureValue(PyList_New((Py_ssize_t) experimentResults.size()));
    Py_ssize_t i = 0;
    for (const auto &entry: experimentResults) {
        pList = secureValue(PyList_New((Py_ssize_t) entry.second.size()));
        Py_ssize_t j = 0;
        for (const auto &id: entry.second) {
            PyList_SetItem(pList, j, secureValue(PyUnicode_FromString(id.c_str())));
            j++;
        }
        PyList_SetItem(pValue, i, pList);
        i++;
    }
    PyTuple_SetItem(pArgs, 2, pValue);
    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    if (pValue) {
        Py_ssize_t j = 0;
        for (const auto &entry: experimentResults) {
            functionValue val = PyFloat_AsDouble(secureValue(PyList_GetItem(pValue, j)));
            result.insert(make_pair(entry, val));
            j++;
        }
        Py_DECREF(pValue);
    } else {
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        PyErr_Print();
        throw runtime_error("Call to Python function failed!");
    }
    return result;
}

functionValue ConstantHeadway::processOutput(filesystem::path path, set<runId> experimentIds, unsigned int pipelineId) {
    return processOutput(set<pair<filesystem::path, set<runId>>>({make_pair(path, experimentIds)}),
                         pipelineId).begin()->second;
}

PyObject *ConstantHeadway::secureValue(PyObject *object) {
    if (!object) {
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        throw invalid_argument("PyObject couldn't be created!");
    }
    return object;
}
