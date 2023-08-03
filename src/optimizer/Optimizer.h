#ifndef SIMOPTICON_OPTIMIZER_H
#define SIMOPTICON_OPTIMIZER_H

#include "../Types.h"
#include "../parameters/ParameterDefinition.h"
#include "../status/Status.h"
#include "../utils/Abortable.h"

#include <list>
#include <vector>
#include <map>
#include <memory>

class Controller;

class Parameter;

class ValueMap;

using namespace std;

/**
 * This module contains components capable of finding the minimum of a function only defined through argument-value pairs.
 * Implementations must extend Optimizer.
 * @defgroup optimizer
 */

/**
 * A class containing an optimization strategy which searches the minimum of a blackbox function given through argument-value pairs.
 * The Optimizer has control over which Parameter combinations are simulated and evaluated as well as the duration of the optimization.
 * If Optimizer::abort is called the optimization strategy should finish the optimization as soon as possible.
 * @ingroup optimizer
 */
class Optimizer : public Status, public Abortable {
private:
    /**
     * Reference to the executing Controller to be able to request values using Controller::requestValues.
     */
    Controller &controller;

protected:
    /**
     * List of parameters to be optimized.
     */
    list<shared_ptr<ParameterDefinition>> parameters;

    /**
     * Requests the values when using certain Parameter combinations from Optimizer::controller.
     * Basically calls Controller::requestValues with the given values.
     * @param params: Parameter combinations to be evaluated.
     * @return A map which maps Parameter combinations to their respective values.
     */
    map<vector<shared_ptr<Parameter>>, functionValue> requestValues(const list<vector<shared_ptr<Parameter>>> &params);

public:
    /**
     * Creates an Optimizer which can request values from the given Controller and tries to optimize the given parameters.
     * @param ctrl: Controller to be used for evaluation of Parameter combinations.
     * @param params: List of ParameterDefinition defining the parameters that must be optimized.
     */
    Optimizer(Controller &ctrl, list<shared_ptr<ParameterDefinition>> params);

    virtual ~Optimizer() = default;

    /**
     * Starts the optimization process.
     * Should only return if the optimization strategy deems the optimization complete or when Optimizer::abort is called.
     */
    virtual void runOptimization() = 0;

    /**
     * Returns a reference to Controller::valueMap. Basically calls Controller::getValueMap on Optimizer::controller.
     * @return
     */
    [[nodiscard]] ValueMap &getValueMap() const;

    string getName() override;

    string getStatus() override;

    string getStatusBar() override;

};


#endif //SIMOPTICON_OPTIMIZER_H
