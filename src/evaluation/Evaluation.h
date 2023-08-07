#ifndef SIMOPTICON_EVALUATION_H
#define SIMOPTICON_EVALUATION_H

/**
 * @dir
 * In this directory, classes of the evaluation module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the Evaluation class is defined.
 */

#include "../Types.h"
#include "../status/Status.h"

#include <set>
#include <filesystem>
#include <map>

/**
 * This module contains components capable of evaluating the performance of simulations by rating simulation data with a number value.
 * Implementations must extend Evaluation.
 * @defgroup evaluation evaluation
 */

/**
 * A class capable of evaluating simulation results and scoring them with a value which is treated as the function value for the optimization.
 * A lower value is considered better in this framework. The optimized function can be viewed as an error function.
 * @ingroup evaluation
 */
class Evaluation : public Status {
public:
    virtual ~Evaluation() = default;

    /**
     * Returns a value to the results of a single simulation run.
     * @param path: Path to the result files.
     * @param experimentIds: Identifiers of certain simulation runs within the directory represented by the given path.
     * @return A value that represents the performance of the simulation - the lower the better.
     */
    virtual functionValue processOutput(std::filesystem::path path, std::set<runId> experimentIds) = 0;

    /**
     * Returns values to the results of multiple simulation runs.
     * Simply calls #processOutput(filesystem::path, set<runId>) multiple times if not overridden.
     * @param experimentResults: Paths to and identifiers of the simulation results.
     * @return A map which maps the given results to their respective performance value.
     */
    virtual std::map<std::pair<std::filesystem::path, std::set<runId>>, functionValue>
    processOutput(const std::set<std::pair<std::filesystem::path, std::set<runId>>> &experimentResults);

    std::string getName() override;

    std::string getStatus() override;

    std::string getStatusBar() override;

};


#endif //SIMOPTICON_EVALUATION_H
