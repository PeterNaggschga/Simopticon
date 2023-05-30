#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H

#include <map>
#include <vector>
#include <list>
#include "Types.h"

class Optimizer;

class SimulationRunner;

class Pipeline;

class Parameter;

using namespace std;

class Controller {
private:
    Optimizer &optimizer;
    SimulationRunner &runner;
    Pipeline &pipeline;
    map<vector<Parameter>, functionValue> values;

public:
    map<vector<Parameter *>, functionValue> requestValues(const list<vector<Parameter *>> &params);

};


#endif //SIMOPTICON_CONTROLLER_H
