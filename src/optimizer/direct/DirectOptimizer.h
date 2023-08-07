#ifndef SIMOPTICON_DIRECTOPTIMIZER_H
#define SIMOPTICON_DIRECTOPTIMIZER_H

/**
 * @dir
 * In this directory, classes of the direct module are defined and implemented.
 */

/**
 * @file
 * In this file, the header of the DirectOptimizer class is defined.
 */

#include "DirectTypes.h"
#include "DirectComparisonFunctions.h"
#include "../Optimizer.h"
#include "StoppingCondition.h"
#include "hyrect/HyRect.h"
#include "ParameterNormalizer.h"
#include "Levels.h"

#include <set>
#include <unordered_set>
#include <memory>

class StoppingCondition;

/**
 * This module extends Optimizer to use a variant of the DIRECT algorithm by <a href="https://doi.org/10.1007/BF00941892">Jones et al</a>.
 * It incorporates features proposed by <a href="https://doi.org/10.1007/s10898-016-0447-z">Liu et al.</a> and <a href="https://doi.org/10.1137/040621132">Sergeyev and Kvasov</a>.
 * @defgroup direct direct
 * @ingroup optimizer
 */

/**
 * A class capable of finding the minimum of a blackbox function using the DIRECT algorithm.
 * @ingroup direct
 */
class DirectOptimizer : public Optimizer {
private:
    /**
     * Number of parameters to be optimized (meaning dimensions of the search space).
     */
    const dimension D;
    /**
     * Number of iterations completed.
     */
    size_t iterations = 0;
    /**
     * An object deciding when the optimization stops.
     */
    StoppingCondition stopCon;
    /**
     * An object used switching between different levels between global and local search.
     */
    Levels level;
    /**
     * An object used for transformation between the unit hypercube used in DIRECT and the actual parameter space.
     */
    ParameterNormalizer normalizer;
    /**
     * Defines if the current number of iterations, evaluations, rectangles and the optimal value should be recorded into a `.csv` file after each iteration.
     * Can be set in config.
     */
    bool trackProgress;
    /**
     * Defines if all found values should be recorded in a `.csv` file after optimization has finished.
     * Can be set in config.
     */
    bool printValues;

    /**
     * Holds all rectangles that are immediate part of the current partition.
     * This includes all rectangles which have not been divided yet.
     * They are grouped by HyRect#t and sorted by HyRect#avgValue which simplifies the search for potentially optimal rectangles in #optimalRectangles.
     */
    std::map<depth, std::set<std::shared_ptr<HyRect>, CmpSharedHyrect>, std::greater<>> activeRects;

    /**
     * Returns the function values at the given points.
     * Basically transforms the given points from dirCoordinates in the hypercube to actual coordinates
     * in the parameter space using #normalizer and calls #requestValues.
     * @param points: List of points in the hypercube to be evaluated.
     * @return A map which maps the given points to their respective values.
     */
    std::map<std::vector<dirCoordinate>, functionValue> getValues(const std::list<std::vector<dirCoordinate>> &points);

    /**
     * Calculates the minimum expected value in a rectangle when the given Lipschitz constant is assumed.
     * @param rect: Rectangle the minimum is searched for.
     * @param k: Lipschitz constant that is assumed in this rectangle.
     * @return A value representing an estimation of the absolute minimum reachable in this rectangle.
     */
    static functionValue estimatedValue(const std::shared_ptr<HyRect> &rect, double k);

    /**
     * Finds potentially optimal rectangles that should be divided in the current iteration.
     * First filters for only the best rectangles of a size from a subset of all #activeRects determined by #level.
     * Then uses GrahamScan to filter after the first condition of the DIRECT algorithm.
     * Finally filters for the second condition of the DIRECT algorithm.
     * @param nrRects: Size of the partition (meaning number of rectangles in #activeRects).
     * @param phi: Value at the current minimum.
     * @return A list of potentially optimal rectangles.
     */
    std::list<std::shared_ptr<HyRect>> optimalRectangles(size_t nrRects, functionValue phi);

    /**
     * Requests values at the corners of the given rectangles and add all given HyRect instances to #activeRects.
     * @param rects: Rectangles to be evaluated and added.
     */
    void addActiveRects(const std::list<std::shared_ptr<HyRect>> &rects);

    /**
     * Removes the given rectangles from #activeRects.
     * @param rects: Rectangles to be removed.
     */
    void removeActiveRects(const std::list<std::shared_ptr<HyRect>> &rects);

    /**
     * Prints the current number of iterations, evaluations, rectangles and the current optimal value to a `.csv` file.
     * @param bestVal: Value at the current minimum.
     * @param evaluations: Number of evaluations conducted by the optimization.
     * @param nrRects: Number of rectangles in the current partition (meaning number of rectangles in #activeRects).
     */
    void saveProgress(functionValue bestVal, size_t evaluations, size_t nrRects) const;

    /**
     * Prints all evaluated parameterCombinations and their respective values to a `.csv` file.
     */
    void saveValues();

public:
    /**
     * Creates a DirectOptimizer that evaluates functions with the given Controller, optimizes the given ParameterDefinition list and stops as defined by the given StoppingCondition.
     * @param ctrl: Controller to be used for evaluating the optimized function.
     * @param params: ParameterDefinition list to be optimized.
     * @param con: StoppingCondition defining the end of optimization.
     * @param trackProgress: Defines whether the progress should be printed in a `.csv` file.
     * @param printValues: Defines whether all obtained values should be printed in a `.csv` file after optimization.
     */
    DirectOptimizer(Controller &ctrl, const std::list<std::shared_ptr<ParameterDefinition>> &params,
                    StoppingCondition con, bool trackProgress, bool printValues);

    /**
     * Starts the optimization using the DIRECT algorithm.
     * Only returns when an iteration has completed and #stopCon deems the optimization complete or when #abort was called in the last iteration.
     */
    void runOptimization() override;

    std::string getName() override;

    std::string getStatus() override;

    std::string getStatusBar() override;

    /**
     * Returns the number of rectangles stored in #activeRects.
     * @return A number representing the size of the partition.
     */
    size_t getPartitionSize();
};


#endif //SIMOPTICON_DIRECTOPTIMIZER_H
