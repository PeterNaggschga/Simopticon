#ifndef SIMOPTICON_SIMULATIONRUNNER_H
#define SIMOPTICON_SIMULATIONRUNNER_H

#include "../Types.h"
#include "../ComparisonFunctions.h"
#include "../utils/Multithreaded.h"
#include "../controller/Status.h"

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <filesystem>
#include <semaphore>
#include <queue>

class Parameter;

using namespace std;

class SimulationRunner
        : public Multithreaded<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>,
          Status {
private:
    pair<filesystem::path, set<runId>> work(vector<shared_ptr<Parameter>> run) override = 0;

public:
    explicit SimulationRunner(unsigned int treads);

    virtual ~SimulationRunner() = default;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs);

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
