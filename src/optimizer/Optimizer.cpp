#include "Optimizer.h"

Optimizer::Optimizer(SimulationController *controller) {
    this->controller = controller;
}

double Optimizer::evaluateFunction() {
    controller->setParams();
    controller->runSimulation();
    return controller->evaluateOutput();
}
