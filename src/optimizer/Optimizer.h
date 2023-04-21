#ifndef SIMOPTICON_OPTIMIZER_H
#define SIMOPTICON_OPTIMIZER_H

#include <list>
#include "../controller/SimulationController.h"
#include "../utils/Parameter.h"

using namespace std;

/**
 * An abstract optimization strategy using parameters of type P.
 */
template<class P> requires is_base_of_v<Parameter, P>
class Optimizer {
    private:
        SimulationController<P> *controller;
        list<P> parameters;

        /**
         * Evaluates the function at the given parameters via the SimulationController.
         * @param params list of simulation parameters
         * @return double value representing the error (the lower the better)
         */
        double evaluateFunction(list<P> params);

    public:
        /**
         * Creates an Optimizer.
         * @param controller SimulationController used for simulation and evaluation
         * @param params list of parameters to be optimized
         */
        explicit Optimizer(SimulationController<P> *controller, list<P> params): controller(controller), parameters(parameters) {};

        virtual ~Optimizer();

        /**
         * Starts the optimization process.
         */
        virtual void optimize() = 0;
};


#endif //SIMOPTICON_OPTIMIZER_H
