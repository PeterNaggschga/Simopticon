/**
 * @file
 * In this file, the implementation of the StatusBar class is defined.
 */

#include "StatusBar.h"

#include "../optimizer/Optimizer.h"

#include <iostream>

const string StatusBar::LARGE_DIVIDER = "\n\n" + string(70, '#') + "\n";

const string StatusBar::SMALL_DIVIDER = string(70, '-') + "\n";

void StatusBar::updateStatus(Status *opt, Status *runner, Status *eval,
                             const pair<parameterCombination, functionValue> &currentVal, bool stepChanged,
                             step currentStep) {
    currentStep = stepChanged ? currentStep : lastStep;
    if (stepChanged || currentVal != lastVal) {
        cout << LARGE_DIVIDER;
        cout << "Current optimum:\n";
        printResult(currentVal.first, currentVal.second);
        cout << SMALL_DIVIDER;
        printStatus(opt);
        cout << SMALL_DIVIDER;
        printStatus(runner);
        cout << SMALL_DIVIDER;
        printStatus(eval);
        cout << SMALL_DIVIDER;
        cout << "Status: ";
        lastStatus = "";
    }
    for (int i = 0; i < lastStatus.size(); ++i) {
        cout << "\b \b";
    }
    switch (currentStep) {
        default:
        case OPTIMIZER:
            lastStatus = opt->getStatusBar();
            break;
        case RUNNER:
            lastStatus = runner->getStatusBar();
            break;
        case EVALUATION:
            lastStatus = eval->getStatusBar();
            break;
    }
    lastVal = currentVal;
    lastStep = currentStep;
    cout << lastStatus;
    cout.flush();
}

void StatusBar::printResult(const parameterCombination &cords, functionValue optimum) {
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

void StatusBar::printResults(list<pair<parameterCombination, pair<functionValue, filesystem::path>>> top) {
    cout << LARGE_DIVIDER;
    size_t i = top.size();
    for (auto it = top.rbegin(); it != top.rend(); ++it) {
        cout << i-- << ". Result\n";
        printResult(it->first, it->second.first);
        cout << "Path to result files: " << it->second.second << "\n";
        cout << SMALL_DIVIDER;
    }
    for (const auto &result: top) {
        cout << ++i << ".\t" << result.second.first << "\n";
    }
    cout.flush();
}
