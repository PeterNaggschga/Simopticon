#include "SimulationController.h"

template<class P> requires is_base_of_v<Parameter, P>
SimulationController<P>::~SimulationController() {
    delete function;
}

template<class P> requires is_base_of_v<Parameter, P>
double SimulationController<P>::evaluateOutput() {
    return function->evaluateOutput();
}
