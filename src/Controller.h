#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H

#include <map>
#include <vector>
#include <list>

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
    map<vector<Parameter>, double> values;

public:
    map<vector<Parameter>, double> getValues(const list<vector<Parameter>> &params);

};


#endif //SIMOPTICON_CONTROLLER_H
