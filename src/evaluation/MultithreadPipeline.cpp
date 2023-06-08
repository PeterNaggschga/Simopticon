#include "MultithreadPipeline.h"

MultithreadPipeline::MultithreadPipeline(unsigned int threads) : Multithreaded(threads) {
}

functionValue MultithreadPipeline::work(tuple<unsigned int, filesystem::path, set<string>> arg) {
    return processOutput(get<0>(arg), get<1>(arg), get<2>(arg));
}

map<tuple<unsigned int, filesystem::path, set<runId>>, functionValue>
MultithreadPipeline::processOutput(const set<tuple<unsigned int, filesystem::path, set<runId>>> &experiments) {
    return runMultithreadedFunctions(experiments);
}
