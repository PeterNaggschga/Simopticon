#include "StatusBar.h"

#include "../optimizer/Optimizer.h"
#include "../runner/SimulationRunner.h"
#include "../evaluation/Evaluation.h"

#include <iostream>
//#include <ranges>

const string StatusBar::LARGE_DIVIDER = "\n\n" + string(70, '#') + "\n";

const string StatusBar::SMALL_DIVIDER = string(70, '-') + "\n";

void StatusBar::updateStatus(Optimizer *opt, SimulationRunner *runner, Evaluation *eval,
                             const pair<vector<shared_ptr<Parameter>>, functionValue> &currentVal, bool stepChanged,
                             step currentStep) {
    currentStep = stepChanged ? currentStep : lastStep;
    if (stepChanged || currentVal != lastVal) {
        cout << LARGE_DIVIDER;
        cout << "Current optimum:\n";
        printResult(currentVal.first, currentVal.second);
        cout << SMALL_DIVIDER;
        printStatus(static_cast<Status *>(opt));
        cout << SMALL_DIVIDER;
        printStatus(static_cast<Status *>(runner));
        cout << SMALL_DIVIDER;
        printStatus(static_cast<Status *>(eval));
        cout << SMALL_DIVIDER;
        cout << "Status: ";
        lastStatus = "";
    }
    for (int i = 0; i < lastStatus.size(); ++i) {
        cout << "\b \b";
    }
    Status *stat;
    switch (currentStep) {
        default:
        case OPTIMIZER:
            stat = static_cast<Status *>(opt);
            break;
        case RUNNER:
            stat = static_cast<Status *>(runner);
            break;
        case EVALUATION:
            stat = static_cast<Status *>(eval);
            break;
    }
    lastStatus = stat->getStatusBar();
    lastVal = currentVal;
    lastStep = currentStep;
    cout << lastStatus;
    cout.flush();
}

void StatusBar::printResult(const vector<shared_ptr<Parameter>> &cords, functionValue optimum) {
    unsigned int i = 1;
    for (const auto &param: cords) {
        if (param->getConfig().empty()) {
            cout << "x" << i;
        } else {
            cout << param->getConfig();
        }
        cout << ":\t" << param->getVal() << param->getUnit() << "\n";
        i++;
    }
    cout << "Value: " << optimum << "\n";
}

void StatusBar::printStatus(Status *object) {
    cout << object->getName() << "\n";
    cout << object->getStatus() << "\n";
}

void StatusBar::printResults(list<pair<vector<shared_ptr<Parameter>>, pair<functionValue, filesystem::path>>> top) {
    cout << LARGE_DIVIDER;
    size_t i = top.size();
    /*
    for (auto &result: std::ranges::reverse_view(top)) {
        cout << i-- << ". Result\n";
        printResult(result.first, result.second.first);
        cout << "Path to result files: " << result.second.second << "\n";
        cout << SMALL_DIVIDER;
    }
     */
    for (const auto &result: top) {
        cout << ++i << ".\t" << result.second.first << "\n";
    }
    cout.flush();
}
