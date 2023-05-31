#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H


#include "../Types.h"

#include <map>
#include <vector>
#include <list>
#include <memory>

class Optimizer;

class SimulationRunner;

class Pipeline;

class Parameter;

class ValueMap;

using namespace std;

class Controller {
private:
    Optimizer &optimizer;
    SimulationRunner &runner;
    Pipeline &pipeline;
    ValueMap &valueMap;

public:
    map<vector<shared_ptr<Parameter>>, functionValue> requestValues(const list<vector<shared_ptr<Parameter>>> &params);

};


#endif //SIMOPTICON_CONTROLLER_H
