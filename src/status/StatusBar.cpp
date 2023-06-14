#include "StatusBar.h"

#include "../optimizer/Optimizer.h"
#include "../runner/SimulationRunner.h"
#include "../evaluation/Pipeline.h"

#include <iostream>

void StatusBar::updateStatus(Optimizer *opt, SimulationRunner *runner, Pipeline *pipe,
                             const pair<vector<shared_ptr<Parameter>>, functionValue> &currentVal, bool stepChanged,
                             step currentStep) {
    currentStep = stepChanged ? currentStep : lastStep;
    if (stepChanged || currentVal != lastVal) {
        if (lastStep != INIT) {
            cout << "\n\n";
        }
        cout << "Irgendein Trenner\n";
        printOptimum(currentVal);
        printStatus(static_cast<Status *>(opt));
        printStatus(static_cast<Status *>(runner));
        printStatus(static_cast<Status *>(pipe));
        cout << "Status: ";
        lastStatus = "";
    }
    for (int i = 0; i < lastStatus.size(); ++i) {
        cout << "\b \b";
    }
    Status *stat;
    switch (currentStep) {
        case INIT:
        case OPTIMIZER:
            stat = static_cast<Status *>(opt);
            break;
        case RUNNER:
            stat = static_cast<Status *>(runner);
            break;
        case PIPELINE:
            stat = static_cast<Status *>(pipe);
            break;
    }
    lastStatus = stat->getStatusBar();
    lastVal = currentVal;
    lastStep = currentStep;
    cout << lastStatus;
    cout.flush();
}

void StatusBar::printOptimum(const pair<vector<shared_ptr<Parameter>>, functionValue> &optimum) {
    cout << "Aktuelles Optimum:\n";
    for (const auto &param: optimum.first) {
        cout << param->getConfig() << ": " << param->getVal() << param->getUnit() << "\n";
    }
    cout << "Wert: " << optimum.second << "\n";
}

void StatusBar::printStatus(Status *object) {
    cout << object->getName() << ":\n";
    cout << object->getStatus() << "\n";
}
