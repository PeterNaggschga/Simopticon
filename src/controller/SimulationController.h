#ifndef SIMOPTICON_SIMULATIONCONTROLLER_H
#define SIMOPTICON_SIMULATIONCONTROLLER_H

#include "../evaluation/PerformanceFunction.h"

/**
 * An abstract controller interfacing with the simulation environment.
 */
class SimulationController {
    private:
        PerformanceFunction *function;

    public:
        /**
         * Creates a SimulationController.
         * @param function The PerformanceFunction used for evaluation.
         */
        explicit SimulationController(PerformanceFunction *function);

        virtual ~SimulationController();

        /**
         * Sets the parameters for the next simulation run.
         */
        virtual void setParams() = 0;

        /**
         * Runs the simulation.
         */
        virtual void runSimulation() = 0;

        /**
         * Use the PerformanceFunction to evaluate the current simulation output.
         * @return A double value representing the error (the lower the better).
         */
        double evaluateOutput();
};


#endif //SIMOPTICON_SIMULATIONCONTROLLER_H
