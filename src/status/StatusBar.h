#ifndef SIMOPTICON_STATUSBAR_H
#define SIMOPTICON_STATUSBAR_H


#include "../Types.h"
#include "../controller/ValueMap.h"
#include "Status.h"

#include <memory>
#include <vector>
#include <filesystem>

class Parameter;
class Optimizer;
class SimulationRunner;

class Evaluation;

class ValueMap;

using namespace std;

enum step : char {
    INIT = -1, OPTIMIZER = 0, RUNNER = 1, EVALUATION = 2
};

class StatusBar {
private:
    const static string LARGE_DIVIDER;
    const static string SMALL_DIVIDER;

    pair<vector<shared_ptr<Parameter>>, functionValue> lastVal;
    step lastStep = INIT;
    string lastStatus;

    static void printResult(const vector<shared_ptr<Parameter>> &cords, functionValue optimum);

    static void printStatus(Status *object);

public:
    void updateStatus(Optimizer *opt, SimulationRunner *runner, Evaluation *eval,
                      const pair<vector<shared_ptr<Parameter>>, functionValue> &currentVal, bool stepChanged = false,
                      step currentStep = INIT);

    static void printResults(list<pair<vector<shared_ptr<Parameter>>, pair<functionValue, filesystem::path>>> top);

};


#endif //SIMOPTICON_STATUSBAR_H
