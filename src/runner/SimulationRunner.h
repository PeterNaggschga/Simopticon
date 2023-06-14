#ifndef SIMOPTICON_SIMULATIONRUNNER_H
#define SIMOPTICON_SIMULATIONRUNNER_H

#include "../Types.h"
#include "../ComparisonFunctions.h"
#include "../utils/Multithreaded.h"
#include "../status/Status.h"

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
          public Status {
private:
    pair<filesystem::path, set<runId>> work(vector<shared_ptr<Parameter>> run) override = 0;

public:
    explicit SimulationRunner(unsigned int treads);

    virtual ~SimulationRunner() = default;

    virtual map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
    runSimulations(const set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> &runs);

    string getName() override;

    string getStatus() override;

    string getStatusBar() override;

};


#endif //SIMOPTICON_SIMULATIONRUNNER_H
