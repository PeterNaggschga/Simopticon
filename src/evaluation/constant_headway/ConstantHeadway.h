#ifndef SIMOPTICON_CONSTANTHEADWAY_H
#define SIMOPTICON_CONSTANTHEADWAY_H

#include "../Pipeline.h"
#include "../../utils/PythonScript.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#pragma clang diagnostic pop

class ConstantHeadway : public Pipeline, public PythonScript {
public:
    ConstantHeadway();

    functionValue processOutput(set<runId> experimentIds, filesystem::path path, unsigned int pipelineId) override;

};

#endif //SIMOPTICON_CONSTANTHEADWAY_H
