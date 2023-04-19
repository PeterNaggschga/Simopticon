#include "Optimizer.h"

template<class P> requires is_base_of_v<Parameter, P>
Optimizer<P>::~Optimizer() {
    delete controller;
}

double Optimizer::evaluateFunction() {
    controller->setParams();
    controller->runSimulation();
    return controller->evaluateOutput();
}
