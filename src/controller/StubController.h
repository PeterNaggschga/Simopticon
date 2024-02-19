#ifndef SIMOPTICON_STUBCONTROLLER_H
#define SIMOPTICON_STUBCONTROLLER_H

/**
 * @file
 * In this file, the header of the StubController class is defined.
 */

#include <functional>
#include "Controller.h"
#include "../optimizer/direct/DirectTypes.h"

/**
 * A class that mocks behaviour of Controller.
 * Instead of real simulations one of the predefined function in #functions is being evaluated, when Controller#requestValues is called.
 * To use StubController instead of Controller a second command line argument has to be passed containing the name of the function to be optimized.
 * The name can be one of the following: quadratic, shekel5, shekel7, shekel10, branin, goldprice, camel6, shubert, hartman3 or hartman6.
 * For more information on all but the first function visit: https://www.sfu.ca/~ssurjano/optimization.html
 * @ingroup controller
 * @author Per Natzschka
 */
class StubController : public Controller {
private:
    /**
     * Map that contains the predefined functions quadratic, shekel5, shekel7, shekel10, branin, goldprice, camel6, shubert, hartman3 and hartman6.
     * For more information on all but the first function visit: https://www.sfu.ca/~ssurjano/optimization.html
     */
    static std::map<std::string, std::function<functionValue(parameterCombination)>> functions;

    /**
     * Function to be optimized in the current optimization. One of the functions in #functions.
     */
    const std::function<functionValue(parameterCombination)> f;

    /**
     * Returns empty paths and runIds for each requested parameterCombination.
     * @param runs: parameterCombination to be simulated.
     * @return Map which maps the given parameterCombinations to empty paths and runIds.
     */
    std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>
    runSimulations(const std::set<parameterCombination, CmpVectorSharedParameter> &runs) override;

    /**
     * Evaluates the given parameterCombinations with #f.
     * @param simulationResults: Map which maps parameterCombinations to empty results (see #runSimulations).
     * @return A Map which maps the given parameterCombinations to the respective value of #f.
     */
    std::map<parameterCombination, functionValue, CmpVectorSharedParameter> evaluate(
            const std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter> &simulationResults) override;

    /**
     * Does nothing, since no simulations are run and therefore no result files are created.
     */
    void removeOldResultfiles() override;

    void updateStatus() override;

public:
    /**
     * Creates a StubController with the given config and function.
     * @param configPath: Path to the main config. Chosen by first command line argument.
     * @param function: Name of the function to be used. Chosen by second command line argument.
     */
    StubController(const std::filesystem::path &configPath, const std::string &function);

};


#endif //SIMOPTICON_STUBCONTROLLER_H
