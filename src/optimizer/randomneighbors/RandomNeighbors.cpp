/**
 * @file
 * In this file, the implementation of the RandomNeighbors optimizer class is defined.
 * @author Burkhard Hensel
 */

#include "RandomNeighbors.h"

#include "../../controller/ValueMap.h"
#include "../../parameters/ContinuousParameter.h"

#include <memory>
#include <fstream>

RandomNeighbors::RandomNeighbors(Controller &ctrl, const std::list<std::shared_ptr<ParameterDefinition>> &params,
                                 nlohmann::json config) : Optimizer(ctrl, params),
                                                          stopCon(StoppingCondition(config.at("stopCon"))),
                                                          trackProgress(config.at("outputProgress")),
                                                          parallelTrials(config.at("evaluations")),
                                                          neighborhoodWidth(config.at("neighborhoodWidth")),
                                                          localSearchProbability(config.at("localSearchProbability")) {
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
void RandomNeighbors::runOptimization() {
    stopCon.setStartNow();

    size_t evaluations = 0;
    functionValue bestValue = INFINITY;

    while (!aborted && stopCon.evaluate(evaluations, bestValue)) {
        std::list<parameterCombination> paramList;

        double mode = (double) rand() / RAND_MAX;
        double size_scale = (double) rand() / RAND_MAX;
        lastLocal = mode < localSearchProbability && iterations >= 5;
        parameterCombination currentOptimum;
        if (lastLocal) {
            currentOptimum = getValueMap().getTopVals().front().first;
        }

        for (int i = 0; i < parallelTrials; i++) {
            auto *pcomb = new parameterCombination();
            coordinate coord;
            int index = 0;
            for (const std::shared_ptr<ParameterDefinition> &pdef: parameters) {
                double rand_relative = (double) rand() / RAND_MAX;
                if (lastLocal) {
                    // local search
                    double val = currentOptimum[index].get()->getVal();
                    double regionSize = size_scale * neighborhoodWidth * (pdef->getMax() - pdef->getMin());
                    coord = val + (rand_relative - 0.5) * regionSize;
                    if (coord < pdef->getMin())
                        coord = pdef->getMin();
                    if (coord > pdef->getMax())
                        coord = pdef->getMax();
                } else {
                    // global search
                    coord = (coordinate) rand_relative * (pdef->getMax() - pdef->getMin()) + pdef->getMin();
                }
                std::shared_ptr<Parameter> newParam(new ContinuousParameter(pdef, coord));
                pcomb->push_back(newParam);
                index++;
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

#pragma clang diagnostic pop

void RandomNeighbors::saveProgress(functionValue bestVal, size_t evaluations) const {
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

std::string RandomNeighbors::getName() {
    return "Random Neighbors";
}

std::string RandomNeighbors::getStatus() {
    std::string status = "Iterations:\t\t\t" + std::to_string(iterations) + "\n";
    status += "Iterations without improvement:\t" + std::to_string(stopCon.getIterationsSinceImprov()) + "\n";
    status += "Last evaluations: " + lastEvaluations;
    status += (lastLocal ? " (local)" : " (global)");
    status += "\n";
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

std::string RandomNeighbors::getStatusBar() {
    return "Optimizer running...";
}

