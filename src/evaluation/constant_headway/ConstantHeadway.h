#ifndef SIMOPTICON_CONSTANTHEADWAY_H
#define SIMOPTICON_CONSTANTHEADWAY_H

#include "../MultithreadPipeline.h"


class ConstantHeadway : public MultithreadPipeline {
public:
    explicit ConstantHeadway(unsigned int threads);

    functionValue processOutput(unsigned int pipelineId, filesystem::path path, set<runId> experimentIds) override;

};

#endif //SIMOPTICON_CONSTANTHEADWAY_H
