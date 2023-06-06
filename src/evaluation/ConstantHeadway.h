#ifndef SIMOPTICON_CONSTANTHEADWAY_H
#define SIMOPTICON_CONSTANTHEADWAY_H

#include "Pipeline.h"

class ConstantHeadway : public Pipeline {
private:
    functionValue value = 0;

public:
    void processOutput(set<runId> experimentIds, filesystem::path path, unsigned int pipelineId) override;

    functionValue getValue() override;
};

#endif //SIMOPTICON_CONSTANTHEADWAY_H
