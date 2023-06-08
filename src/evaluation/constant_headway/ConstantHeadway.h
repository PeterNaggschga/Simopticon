#ifndef SIMOPTICON_CONSTANTHEADWAY_H
#define SIMOPTICON_CONSTANTHEADWAY_H

#include "../../utils/PythonScript.h"
#include "../MultithreadPipeline.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#pragma clang diagnostic pop

class ConstantHeadway : public MultithreadPipeline, PythonScript {
public:
    explicit ConstantHeadway(unsigned int threads);

    functionValue processOutput(unsigned int pipelineId, filesystem::path path, set<runId> experimentIds) override;

};

#endif //SIMOPTICON_CONSTANTHEADWAY_H
