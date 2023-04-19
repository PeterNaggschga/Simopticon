#ifndef SIMOPTICON_SIMULATIONCONTROLLER_H
#define SIMOPTICON_SIMULATIONCONTROLLER_H

#include "../evaluation/PerformanceFunction.h"
#include "../Parameter.h"
#include <list>

using namespace std;

/**
 * An abstract controller interfacing with the simulation environment using parameters of type P.
 */
template<class P> requires is_base_of_v<Parameter, P>
class SimulationController {
    private:
        PerformanceFunction *function;

    public:
        /**
         * Creates a SimulationController.
         * @param function PerformanceFunction used for evaluation
         */
        explicit SimulationController(PerformanceFunction *function): function(function) {};

    virtual ~SimulationController();

        /**
         * Sets the parameters for the next simulation run.
         * \param params list of simulation parameters
         */
        virtual void setParams(std::list<P> params) = 0;

        /**
         * Runs the simulation.
         */
        virtual void runSimulation() = 0;

        /**
         * Use the PerformanceFunction to evaluate the current simulation output.
         * @return double value representing the error (the lower the better)
         */
        double evaluateOutput();
};


#endif //SIMOPTICON_SIMULATIONCONTROLLER_H
