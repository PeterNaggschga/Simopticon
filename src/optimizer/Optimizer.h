#ifndef SIMOPTICON_OPTIMIZER_H
#define SIMOPTICON_OPTIMIZER_H

#include "../controller/SimulationController.h"

/**
 * An abstract optimization strategy.
 */
class Optimizer {
    private:
        SimulationController *controller;

        /**
         * Evaluates the function at the given parameters via the SimulationController.
         * @return A double value representing the error (the lower the better).
         */
        double evaluateFunction();

    public:
        /**
         * Creates an Optimizer.
         * @param controller The SimulationController used for simulation and evaluation.
         */
        explicit Optimizer(SimulationController *controller, list<P> params): controller(controller), params(params) {};

        virtual ~Optimizer();

        /**
         * Starts the optimization process.
         */
        virtual void optimize() = 0;
};


#endif //SIMOPTICON_OPTIMIZER_H
