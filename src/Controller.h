#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H

#include "evaluation/Pipeline.h"
#include "runner/SimulationRunner.h"
#include "parameters/Parameter.h"
#include "optimizer/Optimizer.h"

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
    map<vector<Parameter>, double> getValues(const list<vector<Parameter>> &params);

};

map<vector<Parameter>, double> Controller::getValues(const list<vector<Parameter>> &params) {
    //TODO
    return {};
}


#endif //SIMOPTICON_CONTROLLER_H
