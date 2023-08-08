/**
 * @file
 * In this file, the implementation of the StubController class is defined.
 */

#include "StubController.h"
#include "ValueMap.h"

#include <utility>
#include <cmath>

/**
 * Helper method calculating the Shekel function with @a m local minima for the given input.
 * Only implemented for @f$ 1 \leq m \leq 10 @f$.
 * More information at https://www.sfu.ca/~ssurjano/shekel.html.
 * @param m: Number of local minima.
 * @param v: Argument vector where the function should be evaluated.
 * @return The value of the shekel function at the given argument vector.
 */
functionValue shekel(int m, const parameterCombination &v) {
    const std::vector<std::vector<functionValue>> C = {
            {4, 1, 8, 6, 3, 2, 5, 8, 6, 7},
            {4, 1, 8, 6, 7, 9, 3, 1, 2, 3.6},
            {4, 1, 8, 6, 3, 2, 5, 8, 6, 7},
            {4, 1, 8, 6, 7, 9, 3, 1, 2, 3.6}
    };
    const std::vector<functionValue> beta = {0.1, 0.2, 0.2, 0.4, 0.4, 0.6, 0.3, 0.7, 0.5, 0.5};
    functionValue val = 0;
    for (int i = 0; i < m; ++i) {
        functionValue sum = 0;
        for (int j = 0; j < C.size(); ++j) {
            sum += std::pow(v[j]->getVal() - C[j][i], 2);
        }
        val -= 1 / (sum + beta[i]);
    }
    return val;
}

/**
 * Helper method calculating the Hartman function with the given @a A and @a P matrices for the given input.
 * More information at https://www.sfu.ca/~ssurjano/hart3.html, https://www.sfu.ca/~ssurjano/hart4.html and https://www.sfu.ca/~ssurjano/hart6.html.
 * @param A: Matrix A used in calculation of Hartman function.
 * @param P: Matrix P used in calculation of Hartman function.
 * @param v
 * @return
 */
functionValue hartman(const std::vector<std::vector<functionValue>> &A, const std::vector<std::vector<functionValue>> &P,
                      const parameterCombination &v) {
    const std::vector<functionValue> alpha = {1, 1.2, 3, 3.2};
    functionValue val = 0;
    for (int i = 0; i < alpha.size(); ++i) {
        functionValue sum = 0;
        for (int j = 0; j < A[0].size(); ++j) {
            sum -= A[i][j] * std::pow(v[j]->getVal() - P[i][j], 2);
        }
        val -= alpha[i] * std::exp(sum);
    }
    return val;
}

std::map<std::string, std::function<functionValue(parameterCombination)>> StubController::functions = {
        std::make_pair("quadratic", [](const parameterCombination &v) {
            functionValue val = 0;
            for (const auto &par: v) {
                val += (functionValue) par->getVal() * par->getVal();
            }
            return val;
        }),
        std::make_pair("shekel5", [](const parameterCombination &v) {
            return shekel(5, v);
        }),
        std::make_pair("shekel7", [](const parameterCombination &v) {
            return shekel(7, v);
        }),
        std::make_pair("shekel10", [](const parameterCombination &v) {
            return shekel(10, v);
        }),
        std::make_pair("hartman3", [](const parameterCombination &v) {
            const std::vector<std::vector<functionValue>> A = {
                    {3,   10, 30},
                    {0.1, 10, 35},
                    {3,   10, 30},
                    {0.1, 10, 35}
            };
            const std::vector<std::vector<functionValue>> P = {
                    {0.3689, 0.1170, 0.2673},
                    {0.4699, 0.4387, 0.7470},
                    {0.1091, 0.8732, 0.5547},
                    {0.0381, 0.5743, 0.8828}
            };
            return hartman(A, P, v);
        }),
        std::make_pair("hartman6", [](const parameterCombination &v) {
            const std::vector<std::vector<functionValue>> A = {
                    {10,   3,   17,   3.5, 1.7, 8},
                    {0.05, 10,  17,   0.1, 8,   14},
                    {3,    3.5, 1.7,  10,  17,  8},
                    {17,   8,   0.05, 10,  0.1, 14}
            };
            const std::vector<std::vector<functionValue>> P = {
                    {0.1312, 0.1696, 0.5569, 0.0124, 0.8283, 0.5886},
                    {0.2329, 0.4135, 0.8307, 0.3736, 0.1004, 0.9991},
                    {0.2348, 0.1451, 0.3522, 0.2883, 0.3047, 0.6650},
                    {0.4047, 0.8828, 0.8732, 0.5743, 0.1091, 0.0381}
            };
            return hartman(A, P, v);
        }),
        std::make_pair("branin", [](const parameterCombination &v) {
            functionValue res = std::pow(v[1]->getVal() -
                                         ((functionValue) 5.1 / (4 * std::pow((functionValue) M_PI, 2))) *
                                         std::pow(v[0]->getVal(), 2) + ((functionValue) 5 / M_PI) * v[0]->getVal() - 6,
                                         2);
            return res + (functionValue) 10 * (1 - std::pow(8 * M_PI, -1)) * std::cos(v[0]->getVal()) + 10;
        }),
        std::make_pair("goldprice", [](const parameterCombination &v) {
            functionValue x1 = v[0]->getVal();
            functionValue x2 = v[1]->getVal();
            return (1 + std::pow(x1 + x2 + 1, 2) *
                        (19 - 14 * (x1 + x2) + 3 * (std::pow(x1, 2) + std::pow(x2, 2)) + 6 * x1 * x2)) * (30 + std::pow(
                    2 * x1 - 3 * x2, 2) * (18 - 32 * x1 + 12 * std::pow(x1, 2) + 48 * x2 - 36 * x1 * x2 +
                                           27 * std::pow(x2, 2)));
        }),
        std::make_pair("camel6", [](const parameterCombination &v) {
            functionValue x1 = v[0]->getVal();
            functionValue x2 = v[1]->getVal();
            return (4 - 2.1 * std::pow(x1, 2) + std::pow(x1, 4) / 3) * std::pow(x1, 2) + x1 * x2 +
                   (-4 + 4 * std::pow(x2, 2)) * std::pow(x2, 2);
        }),
        std::make_pair("shubert", [](const parameterCombination &v) {
            functionValue sum1 = 0;
            for (int i = 1; i <= 5; ++i) {
                sum1 += (functionValue) i * std::cos((i + 1) * v[0]->getVal() + i);
            }
            functionValue sum2 = 0;
            for (int i = 1; i <= 5; ++i) {
                sum2 += (functionValue) i * std::cos((i + 1) * v[1]->getVal() + i);
            }
            return sum1 * sum2;
        })
};

StubController::StubController(const std::filesystem::path &configPath, const std::string &function) :
        f(functions[function]), Controller(configPath, true) {
}

std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter>
StubController::runSimulations(const std::set<parameterCombination, CmpVectorSharedParameter> &runs) {
    std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter> result;
    for (const auto &v: runs) {
        result.insert(make_pair(v, make_pair("", std::set<runId>())));
    }
    return result;
}

std::map<parameterCombination, functionValue, CmpVectorSharedParameter> StubController::evaluate(
        const std::map<parameterCombination, std::pair<std::filesystem::path, std::set<runId>>, CmpVectorSharedParameter> &simulationResults) {
    std::map<parameterCombination, functionValue, CmpVectorSharedParameter> result;
    for (const auto &v: simulationResults) {
        result.insert(make_pair(v.first, f(v.first)));
    }
    return result;
}

void StubController::removeOldResultfiles() {
}

void StubController::updateStatus() {
    std::pair<parameterCombination, functionValue> p =
            valueMap->getSize() != 0 ? valueMap->getTopVals().front() : make_pair(parameterCombination(),
                                                                                  (functionValue) INFINITY);
    bool stateChanged = stepState.stepChanged;
    static Status empty = Status();
    statusBar.updateStatus(optimizer.get(), &empty, &empty, p, stateChanged, stepState.get());
}
