#ifndef SIMOPTICON_OPTIMIZER_H
#define SIMOPTICON_OPTIMIZER_H

#include "../Types.h"
#include "../parameters/ParameterDefinition.h"

#include <list>
#include <vector>
#include <map>
#include <memory>

class Controller;

class Parameter;

class ValueMap;

using namespace std;

class Optimizer {
private:
    Controller &controller;
    list<ParameterDefinition> parameters;
    ValueMap &valueMap;

public:
    Optimizer(Controller &ctrl, list<ParameterDefinition> params, ValueMap &map);

    //TODO destructor

    map<vector<shared_ptr<Parameter>>, functionValue> requestValues(const list<vector<shared_ptr<Parameter>>> &params);

    virtual void runOptimization() = 0;
};


#endif //SIMOPTICON_OPTIMIZER_H
