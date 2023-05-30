#ifndef SIMOPTICON_OPTIMIZER_H
#define SIMOPTICON_OPTIMIZER_H

#include "../Types.h"
#include "../parameters/ParameterDefinition.h"

#include <list>
#include <vector>
#include <map>

class Controller;

class Parameter;

using namespace std;

class Optimizer {
private:
    Controller &controller;
    list<ParameterDefinition> parameters;

public:
    Optimizer(Controller &ctrl, list<ParameterDefinition> params);

    //TODO destructor

    map<vector<Parameter>, functionValue> getValues(const list<vector<Parameter>> &params);

    virtual void runOptimization() = 0;
};


#endif //SIMOPTICON_OPTIMIZER_H
