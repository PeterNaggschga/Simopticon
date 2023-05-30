#ifndef SIMOPTICON_CONSTANTHEADWAY_H
#define SIMOPTICON_CONSTANTHEADWAY_H

#include "Pipeline.h"

class ConstantHeadway : public Pipeline {
private:
    functionValue value = 0;

public:
    void processOutput(list<string> experimentIds, string path, int pipelineId) override;

    functionValue getValue() override;
};

#endif //SIMOPTICON_CONSTANTHEADWAY_H
