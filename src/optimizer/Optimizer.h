#ifndef SIMOPTICON_OPTIMIZER_H
#define SIMOPTICON_OPTIMIZER_H

#include <list>
#include "../parameters/ParameterDefinition.h"
#include "../parameters/Parameter.h"
#include "../Controller.h"

using namespace std;

class Optimizer {
private:
    Controller &controller;
    list<ParameterDefinition> parameters;

public:
    Optimizer(Controller &ctrl, list<ParameterDefinition> params);

    //TODO destructor

    map<vector<Parameter>, double> getValues(const list<vector<Parameter>> &params);

    virtual void runOptimization() = 0;
};


#endif //SIMOPTICON_OPTIMIZER_H
