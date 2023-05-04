#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H

#include "optimizer/Optimizer.h"
#include "evaluation/Pipeline.h"
#include "runner/SimulationRunner.h"
#include "parameters/Parameter.h"

#include <map>
#include <vector>

using namespace std;

class Controller {
private:
    Optimizer &optimizer;
    SimulationRunner &runner;
    Pipeline &pipeline;
    map<vector<Parameter>, double> values;

public:
    map<vector<Parameter>, double> getValues(list<vector<Parameter>>);

};


#endif //SIMOPTICON_CONTROLLER_H
