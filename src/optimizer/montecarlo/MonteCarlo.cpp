/**
 * @file
 * In this file, the implementation of the MonteCarlo optimizer class is defined.
 */

#include "MonteCarlo.h"

#include "../../controller/ValueMap.h"
#include "../../parameters/ContinuousParameter.h"

#include <memory>
#include <fstream>

MonteCarlo::MonteCarlo(Controller &ctrl, const std::list<std::shared_ptr<ParameterDefinition>> &params,
                       nlohmann::json config) : Optimizer(ctrl, params),
                                                stopCon(StoppingCondition(config.at("stopCon"))),
                                                trackProgress(config.at("outputProgress")),
                                                parallelTrials(config.at("evaluations")) {
}

void MonteCarlo::runOptimization() {
    stopCon.setStartNow();

    size_t evaluations = 0;
    functionValue bestValue = getValueMap().getTopVals().front().second;
    if (trackProgress) {
        saveProgress(bestValue, evaluations);
    }

    // set random seed for reproducible results
    // NOTE: Separate random number generators would be better if there are different positions in Simopticon where
    // random numbers are used. This is currently (2023-12-07) not the case.
    srand(1234); // NOLINT(*-msc51-cpp)


    while (!aborted && stopCon.evaluate(evaluations, bestValue)) {
        std::list<parameterCombination> paramList;

        // approach taken from parameterNormalizer::denormalize()
        for (int i = 0; i < parallelTrials; i++) {
            auto *pcomb = new parameterCombination();
            for (const std::shared_ptr<ParameterDefinition> &pdef: parameters) {
                double rand_relative = (double) rand() / RAND_MAX; // NOLINT(*-msc50-cpp)
                std::shared_ptr<Parameter> newParam(
                        new ContinuousParameter(pdef, (coordinate) rand_relative * (pdef->getMax() - pdef->getMin()) +
                                                      pdef->getMin()));
                pcomb->push_back(newParam);
            }
            paramList.push_back(*pcomb);
        }
        std::map<parameterCombination, functionValue> values = requestValues(paramList);
        bestValue = getValueMap().getTopVals().front().second;

        lastEvaluations = "";
        for (const parameterCombination &comb: paramList) {
            lastEvaluations += std::to_string(values[comb]) + "; ";
        }


        // update counters
        iterations++;
        evaluations = getValueMap().getSize();
        evaluations += parallelTrials;
        if (trackProgress) {
            saveProgress(bestValue, evaluations);
        }
    }
}

void MonteCarlo::saveProgress(functionValue bestVal, size_t evaluations) const {
    static std::ofstream out;
    if (!out.is_open()) {
        if (!std::filesystem::exists("results/progress.csv")) {
            std::filesystem::create_directories("results");
        }
        out.open("results/progress.csv", std::ios::out | std::ios::trunc);
        out << "Iteration;" << "Evaluations;" << "Minimum\n";
    }
    out << iterations << ";" << evaluations << ";" << bestVal << std::endl;
}

std::string MonteCarlo::getName() {
    return "Monte-Carlo";
}

std::string MonteCarlo::getStatus() {
    std::string status = "Iterations:\t\t\t" + std::to_string(iterations) + "\n";
    status += "Iterations without improvement:\t" + std::to_string(stopCon.getIterationsSinceImprov()) + "\n";
    status += "Last evaluations: " + lastEvaluations + "\n";
    status += "Best evaluations: ";
    int counter = 0;
    for (const auto &val: getValueMap().getTopVals()) {
        status += std::to_string(val.second) + "; ";
        counter++;
        if (counter == 4)
            break;
    }
    return status;
}

std::string MonteCarlo::getStatusBar() {
    return "Optimizer running...";
}

