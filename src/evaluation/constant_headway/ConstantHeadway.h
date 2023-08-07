#ifndef SIMOPTICON_CONSTANTHEADWAY_H
#define SIMOPTICON_CONSTANTHEADWAY_H

/**
 * @dir
 * In this directory, classes of the constant_headway module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the ConstantHeadway class is defined.
 */

#include "../Evaluation.h"
#include "../../utils/PythonScript.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#pragma clang diagnostic pop

/**
 * This module extends Evaluation to interface with a Python script evaluating the performance of platooning simulations with Plexe by analyzing the deviation of vehicles from the pre-specified gap.
 * @defgroup constant_headway constant_headway
 * @ingroup evaluation
 */

/**
 * A wrapper for the constant_headway.py script.
 * @ingroup constant_headway
 */
class ConstantHeadway : public Evaluation, public PythonScript {
private:
    /**
     * Maximum number of threads to use for concurrent evaluation.
     * Can be set in config.
     */
    const unsigned int NR_THREADS;
    /**
     * Number of threads currently used for concurrent evaluation.
     * Used in #getStatusBar.
     */
    unsigned int usedThreads = 0;

    /**
     * Helper function checking if the given object is a null-pointer.
     * If so the constant_headway.py script is disconnected and an error is thrown.
     * @param object: Pointer to PyObject that must be tested.
     * @return The given pointer, if no error was thrown.
     */
    PyObject *secureValue(PyObject *object);

public:
    /**
     * Creates a ConstantHeadway object that uses no more than the given number of threads and interfaces with the multithreaded function of the given script.
     * @param nrThreads: Maximum number of threads used for concurrent calculations.
     * @param pathToScript: Path to the constant_headway.py script.
     */
    ConstantHeadway(unsigned int nrThreads, const std::filesystem::path &pathToScript);

    /**
     * Returns a value to the results of a single simulation run.
     * Basically calls #processOutput(const set<pair<filesystem::path, set<runId>>> &) with the given values.
     * @param path: Path to the result files.
     * @param experimentIds: Identifiers of certain simulation runs within the directory represented by the given path.
     * @return A value that represents the performance of the simulation - the lower the better.
     */
    functionValue processOutput(std::filesystem::path path, std::set<runId> experimentIds) override;

    /**
     * Returns values to the results of multiple simulation runs.
     * Passes given parameters to the multithreaded function of constant_headway.py.
     * @param experimentResults: Paths to and identifiers of the simulation results.
     * @return A map which maps the given results to their respective performance value.
     */
    std::map<std::pair<std::filesystem::path, std::set<runId>>, functionValue>
    processOutput(const std::set<std::pair<std::filesystem::path, std::set<runId>>> &experimentResults) override;

    std::string getName() override;

    std::string getStatus() override;

    std::string getStatusBar() override;

};

#endif //SIMOPTICON_CONSTANTHEADWAY_H
