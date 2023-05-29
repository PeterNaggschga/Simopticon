#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H

#include "evaluation/Pipeline.h"
#include "runner/SimulationRunner.h"
#include "parameters/Parameter.h"

#include <map>
#include <vector>
#include <list>

using namespace std;

class Optimizer;

class Controller {
private:
    Optimizer &optimizer;
    SimulationRunner &runner;
    Pipeline &pipeline;
    map<vector<Parameter>, double> values;

public:
    map<vector<Parameter>, double> getValues(const list <vector<Parameter>> &params);

};


#endif //SIMOPTICON_CONTROLLER_H
