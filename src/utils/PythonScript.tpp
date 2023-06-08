#include <stdexcept>

using namespace std;

template<class RETURN>
RETURN
PythonScript::executePythonFunction(string path, string scriptName, string functionName, vector<string> stringArgs,
                                    vector<long long int> longArgs, vector<double> doubleArgs) {
    RETURN val;
    setenv("PYTHONPATH", path.c_str(), 1);

    //Py_Initialize();
    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;

    pName = PyUnicode_DecodeFSDefault(scriptName.c_str());
    if (pName) {
        pModule = PyImport_Import(pName);
        Py_DECREF(pName);
    } else {
        throw invalid_argument("Name of Python Script invalid!");
    }

    if (pModule) {
        pFunc = PyObject_GetAttrString(pModule, functionName.c_str());

        if (pFunc && PyCallable_Check(pFunc)) {
            size_t nrArgs = stringArgs.size() + longArgs.size() + doubleArgs.size();
            pArgs = PyTuple_New((Py_ssize_t) nrArgs);
            for (int i = 0; i < nrArgs; ++i) {
                if (i < stringArgs.size()) {
                    pValue = PyUnicode_FromString(stringArgs[i].c_str());
                } else if (i < stringArgs.size() + longArgs.size()) {
                    pValue = PyLong_FromLongLong(longArgs[i - stringArgs.size()]);
                } else {
                    pValue = PyFloat_FromDouble(doubleArgs[i - stringArgs.size() - longArgs.size()]);
                }
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    throw invalid_argument("Python argument couldn't be created!");
                }
                PyTuple_SetItem(pArgs, i, pValue);
            }

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue) {
                if (is_integral<RETURN>::value) {
                    val = (RETURN) PyLong_AsLongLong(pValue);
                } else if (is_floating_point<RETURN>::value) {
                    val = (RETURN) PyFloat_AsDouble(pValue);
                }
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
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            throw runtime_error("Function couldn't be loaded!");
        }
    } else {
        PyErr_Print();
        throw runtime_error("Module couldn't be loaded!");
    }

    Py_Finalize();
    return val;
}
