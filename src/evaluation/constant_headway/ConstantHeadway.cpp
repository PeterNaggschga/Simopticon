/**
 * @file
 * In this file, the implementation of the ConstantHeadway class is defined.
 * @author Per Natzschka
 */

#include "ConstantHeadway.h"

#include <iostream>

ConstantHeadway::ConstantHeadway(unsigned int nrThreads, const std::filesystem::path &pathToScript) : PythonScript(
        pathToScript, "multithreaded"), NR_THREADS(nrThreads) {
}

std::map<std::pair<std::filesystem::path, std::set<runId>>, functionValue>
ConstantHeadway::processOutput(const std::set<std::pair<std::filesystem::path, std::set<runId>>> &experimentResults) {
    usedThreads = std::min((size_t) NR_THREADS, experimentResults.size());
    std::map<std::pair<std::filesystem::path, std::set<runId>>, functionValue> result;

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
            char *bytes = PyByteArray_AsString(PyByteArray_FromObject(secureValue(PyList_GetItem(pValue, j))));
            functionValue val;
            memmove(&val, bytes, sizeof(functionValue));
            result.insert(make_pair(entry, val));
            j++;
        }
        Py_DECREF(pValue);
    } else {
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        std::cerr << "Call to Python function failed!";
        return {};
    }
    usedThreads = 0;
    return result;
}

functionValue ConstantHeadway::processOutput(std::filesystem::path path, std::set<runId> experimentIds) {
    return processOutput(std::set<std::pair<std::filesystem::path, std::set<runId>>>(
            {make_pair(path, experimentIds)})).begin()->second;
}

PyObject *ConstantHeadway::secureValue(PyObject *object) {
    if (!object) {
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        std::cerr << "PyObject couldn't be created!" << std::endl;
        return nullptr;
    }
    return object;
}

std::string ConstantHeadway::getName() {
    return "Constant-Headway-Evaluation";
}

std::string ConstantHeadway::getStatus() {
    return "Max. number of threads: " + std::to_string(NR_THREADS);
}

std::string ConstantHeadway::getStatusBar() {
    return "Evaluating results... using " + std::to_string(usedThreads) + "/" + std::to_string(NR_THREADS) + " threads";
}
