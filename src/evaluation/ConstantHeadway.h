#ifndef SIMOPTICON_CONSTANTHEADWAY_H
#define SIMOPTICON_CONSTANTHEADWAY_H

#include "Pipeline.h"

class ConstantHeadway : public Pipeline {
private:
    double value = 0;

public:
    void processOutput(list<string> experimentIds, string path, int pipelineId) override;

    double getValue() override;
};

#endif //SIMOPTICON_CONSTANTHEADWAY_H
