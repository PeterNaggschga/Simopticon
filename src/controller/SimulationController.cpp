#include "SimulationController.h"

SimulationController::SimulationController(PerformanceFunction *function) {
    this->function = function;
}

double SimulationController::evaluateOutput() {
    return function->evaluateOutput();
}
