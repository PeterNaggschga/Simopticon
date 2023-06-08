#include "ConstantHeadway.h"

#include "../../utils/PythonScript.h"

#include <string>
#include <cstring>

using namespace std;

ConstantHeadway::ConstantHeadway(unsigned int threads) : MultithreadPipeline(threads) {
}

functionValue ConstantHeadway::processOutput(unsigned int pipelineId, filesystem::path path, set<runId> experimentIds) {
    return PythonScript::executePythonFunction<functionValue>(
            strcat(getenv("SIMOPTICON_HOME"), "/src/evaluation/constant_headway"), "constant_headway",
            "get_constant_headway", vector<string>({path.string()}));
}

