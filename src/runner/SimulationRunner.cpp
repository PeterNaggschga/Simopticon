#include "SimulationRunner.h"

#include <cmath>
#include <future>

SimulationRunner::SimulationRunner(unsigned int threads, unsigned int runs) : NR_THREADS(threads),
                                                                              NR_RUNS_PER_THREAD(runs) {
}

map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>
SimulationRunner::runSimulations(set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> runs) {
    unsigned int nrThreadRuns = ceil((double) runs.size() / NR_RUNS_PER_THREAD);
    set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> threadRuns[nrThreadRuns];
    for (int i = 0; i < nrThreadRuns; ++i) {
        auto it = runs.begin();
        advance(it, NR_RUNS_PER_THREAD < runs.size() ? NR_RUNS_PER_THREAD : runs.size());
        threadRuns[i].insert(runs.begin(), it);
        runs.erase(runs.begin(), it);
    }

    map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter> result;
    vector<future<map<vector<shared_ptr<Parameter>>, pair<filesystem::path, set<runId>>, CmpVectorSharedParameter>>> threads;
    int i = 0;
    counting_semaphore<SEMAPHORE_MAX> semaphore(0);
    while (result.size() < nrRuns) {
        if (threads.size() < NR_THREADS && i < nrThreadRuns) {
            threads.push_back(async(std::launch::async, &SimulationRunner::runSimulationThread, this, threadRuns[i++],
                                    &semaphore));
        } else {
            semaphore.acquire();
            auto it = threads.begin();
            while (it != threads.end()) {
                if (it->wait_for(chrono::seconds(0)) == std::future_status::ready) {
                    auto help = it->get();
                    result.insert(help.begin(), help.end());
                    threads.erase(it);
                    break;
                }
                it++;
            }
        }
    }

    return result;
}
