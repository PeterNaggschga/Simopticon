#ifndef SIMOPTICON_STATUSBAR_H
#define SIMOPTICON_STATUSBAR_H


#include "../Types.h"
#include "../controller/ValueMap.h"
#include "Status.h"

#include <memory>
#include <vector>

class Parameter;

class Optimizer;

class SimulationRunner;

class Pipeline;

class ValueMap;

using namespace std;

enum step : char {
    INIT = -1, OPTIMIZER = 0, RUNNER = 1, PIPELINE = 2
};

class StatusBar {
private:
    pair<vector<shared_ptr<Parameter>>, functionValue> lastVal;
    step lastStep = INIT;
    string lastStatus;

    static void printOptimum(const pair<vector<shared_ptr<Parameter>>, functionValue> &optimum);

    static void printStatus(Status *object);

public:
    void updateStatus(Optimizer *opt, SimulationRunner *runner, Pipeline *pipe,
                      const pair<vector<shared_ptr<Parameter>>, functionValue> &currentVal, step currentStep);

};


#endif //SIMOPTICON_STATUSBAR_H
