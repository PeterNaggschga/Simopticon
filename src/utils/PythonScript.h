#ifndef SIMOPTICON_PYTHONSCRIPT_H
#define SIMOPTICON_PYTHONSCRIPT_H

/**
 * @file
 * In this file the header of the PythonScript class is defined.
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <filesystem>

#pragma clang diagnostic pop

/**
 * A class containing functionality for interfacing with the function of a Python module on creation.
 * See https://docs.python.org/3/c-api/index.html for more information.
 * @ingroup utils
 */
class PythonScript {
protected:
    /**
     * Pointer to module that contains function which should be used by the class.
     */
    PyObject *pModule;
    /**
     * Pointer to function which should be used by the class.
     */
    PyObject *pFunc;

public:
    /**
     * Creates a connection to the given function of a Python script at the given path.
     * @param path: Path to the Python script containing the function.
     * @param functionName: Name of the function to be used.
     */
    PythonScript(const std::filesystem::path &path, const char *functionName);

    /**
     * Ends connection to function #pFunc and module #pModule.
     */
    ~PythonScript();

};


#endif //SIMOPTICON_PYTHONSCRIPT_H
