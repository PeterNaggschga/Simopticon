#include "SimulationController.h"

SimulationController::SimulationController(PerformanceFunction *function) {
    this->function = function;
}

SimulationController::~SimulationController() {
    delete function;
}

double SimulationController::evaluateOutput() {
    return function->evaluateOutput();
}
