#include "Optimizer.h"

template<class P> requires is_base_of_v<Parameter, P>
Optimizer<P>::~Optimizer() {
    delete controller;
}

template<class P>requires is_base_of_v<Parameter, P>
double Optimizer<P>::evaluateFunction(list<P> params) {
    controller->setParams(params);
    controller->runSimulation();
    return controller->evaluateOutput();
}
