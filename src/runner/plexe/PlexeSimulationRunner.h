#ifndef SIMOPTICON_PLEXESIMULATIONRUNNER_H
#define SIMOPTICON_PLEXESIMULATIONRUNNER_H


#include "../SimulationRunner.h"
#include "ConfigEditor.h"

#include <list>
#include <mutex>
#include <semaphore>

class PlexeSimulationRunner : public SimulationRunner {
private:
    const unsigned int REPEAT;
    const vector<string> SCENARIOS;

    ConfigEditor editor;

    size_t runNumber = 0;
    mutex runNumberLock;

    size_t getRunId();

    pair<filesystem::path, set<runId>> work(vector<shared_ptr<Parameter>> run) override;

public:
    PlexeSimulationRunner(unsigned int threads, unsigned int repeat, vector<string> scenarios, ConfigEditor editor);

};


#endif //SIMOPTICON_PLEXESIMULATIONRUNNER_H
