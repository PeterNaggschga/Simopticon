#include "PlexeSimulationRunner.h"
#include "ConfigEditor.h"

#include <utility>

PlexeSimulationRunner::PlexeSimulationRunner(unsigned int threads, unsigned int runs, unsigned int repeat,
                                             vector<string> scenarios, ConfigEditor editor) : SimulationRunner(threads,
                                                                                                               runs /
                                                                                                               (repeat *
                                                                                                                scenarios.size()) +
                                                                                                               1),
                                                                                              REPEAT(repeat), SCENARIOS(
                std::move(scenarios)), editor(std::move(editor)) {
}

map<vector<shared_ptr<Parameter>>, runId, CmpVectorSharedParameter>
PlexeSimulationRunner::runSimulationThread(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) {
    map<vector<shared_ptr<Parameter>>, unsigned long, CmpVectorSharedParameter> runToId;

    // TODO: Sinusoidal und Braking starten (bzw. abrufen, was gestartet werden soll)

    return {};
}

unsigned long PlexeSimulationRunner::getRunId() {
    runNumberLock.lock();
    unsigned long result = runNumber;
    runNumber++;
    runNumberLock.unlock();
    return result;
}
