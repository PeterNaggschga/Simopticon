#ifndef SIMOPTICON_MULTITHREADPIPELINE_H
#define SIMOPTICON_MULTITHREADPIPELINE_H


#include "Pipeline.h"
#include "../utils/Multithreaded.h"

class MultithreadPipeline
        : public Pipeline, Multithreaded<tuple<unsigned int, filesystem::path, set<runId>>, functionValue> {
private:
    functionValue work(tuple<unsigned int, filesystem::path, set<string>> arg) override;

protected:
    explicit MultithreadPipeline(unsigned int threads);

public:
    functionValue processOutput(unsigned int pipelineId, filesystem::path path, set<runId> experimentIds) override = 0;

    map<tuple<unsigned int, filesystem::path, set<runId>>, functionValue> processOutput(
            const set<tuple<unsigned int, filesystem::path, set<runId>>> &experiments) override;
};


#endif //SIMOPTICON_MULTITHREADPIPELINE_H
