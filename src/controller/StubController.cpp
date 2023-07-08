#include "StubController.h"
#include "ValueMap.h"

#include <utility>
#include <cmath>

functionValue shekel(int m, const vector<shared_ptr<Parameter>> &v) {
    const vector<vector<functionValue>> C = {
            {4, 1, 8, 6, 3, 2, 5, 8, 6, 7},
            {4, 1, 8, 6, 7, 9, 3, 1, 2, 3.6},
            {4, 1, 8, 6, 3, 2, 5, 8, 6, 7},
            {4, 1, 8, 6, 7, 9, 3, 1, 2, 3.6}
    };
    const vector<functionValue> beta = {0.1, 0.2, 0.2, 0.4, 0.4, 0.6, 0.3, 0.7, 0.5, 0.5};
    functionValue val = 0;
    for (int i = 0; i < m; ++i) {
        functionValue sum = 0;
        for (int j = 0; j < C.size(); ++j) {
            sum += pow(v[j]->getVal() - C[j][i], 2);
        }
        val -= 1 / (sum + beta[i]);
    }
    return val;
}

functionValue hartman(const vector<vector<functionValue>> &A, const vector<vector<functionValue>> &P,
                      const vector<shared_ptr<Parameter>> &v) {
    const vector<functionValue> alpha = {1, 1.2, 3, 3.2};
    functionValue val = 0;
    for (int i = 0; i < alpha.size(); ++i) {
        functionValue sum = 0;
        for (int j = 0; j < A[0].size(); ++j) {
            sum -= A[i][j] * pow(v[j]->getVal() - P[i][j], 2);
        }
        val -= alpha[i] * exp(sum);
    }
    return val;
}

map<string, function<functionValue(vector<shared_ptr<Parameter>>)>> StubController::functions = {
        make_pair("quadratic", [](const vector<shared_ptr<Parameter>> &v) {
            functionValue val = 0;
            for (const auto &par: v) {
                val += par->getVal() * par->getVal();
            }
            return val;
        }),
        make_pair("shekel5", [](const vector<shared_ptr<Parameter>> &v) {
            return shekel(5, v);
        }),
        make_pair("shekel7", [](const vector<shared_ptr<Parameter>> &v) {
            return shekel(7, v);
        }),
        make_pair("shekel10", [](const vector<shared_ptr<Parameter>> &v) {
            return shekel(10, v);
        }),
        make_pair("hartman3", [](const vector<shared_ptr<Parameter>> &v) {
            const vector<vector<functionValue>> A = {
                    {3,   10, 30},
                    {0.1, 10, 35},
                    {3,   10, 30},
                    {0.1, 10, 35}
            };
            const vector<vector<functionValue>> P = {
                    {0.3689, 0.1170, 0.2673},
                    {0.4699, 0.4387, 0.7470},
                    {0.1091, 0.8732, 0.5547},
                    {0.0381, 0.5743, 0.8828}
            };
            return hartman(A, P, v);
        }),
        make_pair("hartman6", [](const vector<shared_ptr<Parameter>> &v) {
            const vector<vector<functionValue>> A = {
                    {10,   3,   17,   3.5, 1.7, 8},
                    {0.05, 10,  17,   0.1, 8,   14},
                    {3,    3.5, 1.7,  10,  17,  8},
                    {17,   8,   0.05, 10,  0.1, 14}
            };
            const vector<vector<functionValue>> P = {
                    {0.1312, 0.1696, 0.5569, 0.0124, 0.8283, 0.5886},
                    {0.2329, 0.4135, 0.8307, 0.3736, 0.1004, 0.9991},
                    {0.2348, 0.1451, 0.3522, 0.2883, 0.3047, 0.6650},
                    {0.4047, 0.8828, 0.8732, 0.5743, 0.1091, 0.0381}
            };
            return hartman(A, P, v);
        }),
        make_pair("branin", [](const vector<shared_ptr<Parameter>> &v) {
            functionValue res = pow(v[1]->getVal() - ((functionValue) 5.1 / (4 * pow((functionValue) M_PI, 2))) *
                                                     pow(v[0]->getVal(), 2) +
                                    ((functionValue) 5 / M_PI) * v[0]->getVal() - 6, 2);
            return res + 10 * (1 - pow(8 * M_PI, -1)) * cos(v[0]->getVal()) + 10;
        }),
        make_pair("goldprice", [](const vector<shared_ptr<Parameter>> &v) {
            functionValue x1 = v[0]->getVal();
            functionValue x2 = v[1]->getVal();
            return (1 + pow(x1 + x2 + 1, 2) * (19 - 14 * (x1 + x2) + 3 * (pow(x1, 2) + pow(x2, 2)) + 6 * x1 * x2)) *
                   (30 + pow(2 * x1 - 3 * x2, 2) * (18 - 32 * x1 + 12 *
                                                                   pow(x1, 2) + 48 * x2 - 36 * x1 * x2 +
                                                    27 * pow(x2, 2)));
        }),
        make_pair("camel6", [](const vector<shared_ptr<Parameter>> &v) {
            functionValue x1 = v[0]->getVal();
            functionValue x2 = v[1]->getVal();
            return (4 - 2.1 * pow(x1, 2) + pow(x1, 4) / 3) * pow(x1, 2) + x1 * x2 + (-4 + 4 * pow(x2, 2)) * pow(x2, 2);
        }),
        make_pair("shubert", [](const vector<shared_ptr<Parameter>> &v) {
            functionValue sum1 = 0;
            for (int i = 1; i <= 5; ++i) {
                sum1 += i * cos((i + 1) * v[0]->getVal() + i);
            }
            functionValue sum2 = 0;
            for (int i = 1; i <= 5; ++i) {
                sum2 += i * cos((i + 1) * v[1]->getVal() + i);
            }
            return sum1 * sum2;
        })
};

StubController::StubController(const filesystem::path &configPath, const string &function) : f(functions[function]),
                                                                                             Controller(configPath) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
StubController::runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs) {
    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> result;
    for (const auto &v: runs) {
        result.insert(make_pair(v, make_pair("", set<runId>())));
    }
    return result;
}

map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> StubController::evaluate(
        const map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> &simulationResults) {
    map<vector<shared_ptr<Parameter>>, functionValue, CmpVectorSharedParameter> result;
    for (const auto &v: simulationResults) {
        result.insert(make_pair(v.first, f(v.first)));
    }
    return result;
}

void StubController::removeOldResultfiles() {
}
