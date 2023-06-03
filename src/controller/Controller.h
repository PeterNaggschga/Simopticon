#ifndef SIMOPTICON_CONTROLLER_H
#define SIMOPTICON_CONTROLLER_H


#include "../Types.h"
#include "../optimizer/Optimizer.h"
#include "../runner/SimulationRunner.h"
#include "../evaluation/Pipeline.h"
#include "../parameters/Parameter.h"

#include <map>
#include <vector>
#include <list>
#include <memory>
#include <set>

class ParameterDefinition;

class ValueMap;

using namespace std;

class Controller {
protected:
    struct ParPtrCmp {
        bool operator()(vector<shared_ptr<Parameter>> a, vector<shared_ptr<Parameter>> b) const {
            if (a.size() != b.size()) {
                return a.size() < b.size();
            }
            for (int i = 0; i < a.size(); ++i) {
                if (*a[i] != *b[i]) {
                    return *a[i] < *b[i];
                }
            }
            return false;
        }
    };

private:
    unique_ptr<Optimizer> optimizer;
    unique_ptr<SimulationRunner> runner;
    unique_ptr<Pipeline> pipeline;
    unique_ptr<ValueMap> valueMap;

    virtual void runSimulations(set<vector<shared_ptr<Parameter>>, ParPtrCmp> runs);

    virtual map<vector<shared_ptr<Parameter>>, functionValue> evaluate();


public:
    explicit Controller(const list<shared_ptr<ParameterDefinition>> &params);

    void run();

    map<vector<shared_ptr<Parameter>>, functionValue> requestValues(const list<vector<shared_ptr<Parameter>>> &params);

    [[nodiscard]] ValueMap &getValueMap();
};


#endif //SIMOPTICON_CONTROLLER_H
