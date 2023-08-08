#ifndef SIMOPTICON_STATUSBAR_H
#define SIMOPTICON_STATUSBAR_H

/**
 * @file
 * In this file, the header of the StatusBar class is defined.
 */

#include "../Types.h"
#include "../controller/ValueMap.h"
#include "Status.h"

#include <memory>
#include <vector>
#include <filesystem>

class Parameter;
class Optimizer;
class SimulationRunner;

class Evaluation;

class ValueMap;

/**
 * An Enum defining the steps, an optimization process cycles through.
 * @ingroup status
 */
enum step : char {
    INIT = -1, OPTIMIZER = 0, RUNNER = 1, EVALUATION = 2
};

/**
 * A class used to conduct command line output containing information about the state of the used Optimizer, SimulationRunner and Evaluation along with the found optima.
 * @ingroup status
 */
class StatusBar {
private:
    /**
     * Large divider used to visibly divide two sections of content.
     */
    const static std::string LARGE_DIVIDER;
    /**
     * Small divider used to visibly divide two sections of content.
     */
    const static std::string SMALL_DIVIDER;

    /**
     * Pair of parameterCombination and respective value used to discern if the best value has changed since the last call to #updateStatus.
     */
    std::pair<parameterCombination, functionValue> lastVal;
    /**
     * Step which the optimization was in when #updateStatus was called the last time.
     */
    step lastStep = INIT;
    /**
     * Last values of the StatusBar output (excluding value returned by Status#getStatusBar)
     */
    std::string lastStatus;

    /**
     Prints the given result command line.
     * @param cords: parameterCombination of the given result.
     * @param optimum: Value of the given result.
     */
    static void printResult(const parameterCombination &cords, functionValue optimum);

    /**
     * Prints the Status of the given object to the command line using Status#getStatus.
     * @param object: Object that inherits from Status and whose state is being printed.
     */
    static void printStatus(Status *object);

public:
    /**
     * Updates the output in the command line with gathered information from the used Optimizer, SimulationRunner and Evaluation.
     * If the current optimum or the step the optimization is in has changed since the last call, the whole output is printed again.
     * Otherwise only the progress of the active component obtained by Status#getStatusBar is updated.
     * @param opt: Pointer to Optimizer used in optimization.
     * @param runner: Pointer to SimulationRunner used in optimization.
     * @param eval: Pointer to Evaluation used in optimization.
     * @param currentVal: parameterCombination and respective value of the current optimum.
     * @param stepChanged: Boolean defining whether the current step has changed since the last call.
     * @param currentStep: Current step the optimization is in.
     */
    void updateStatus(Status *opt, Status *runner, Status *eval,
                      const std::pair<parameterCombination, functionValue> &currentVal, bool stepChanged = false,
                      step currentStep = INIT);

    /**
     * Prints the given parameterCombinations and respective values to command line.
     * @param top: List of parameterCombinations and respective values to be printed.
     */
    static void
    printResults(std::list<std::pair<parameterCombination, std::pair<functionValue, std::filesystem::path>>> top);

};


#endif //SIMOPTICON_STATUSBAR_H
