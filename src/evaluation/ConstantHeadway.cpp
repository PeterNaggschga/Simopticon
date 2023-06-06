#include "ConstantHeadway.h"
#include "../utils/CommandLine.h"
#include <string>

using namespace std;

void ConstantHeadway::processOutput(set<runId> experimentIds, filesystem::path path, unsigned int pipelineId) {
    string pipeline = CommandLine::SOURCE + "src/evaluation/pipeline/pipeline.py ";
    string cmd = pipeline + "-p " + path.string();
    string out = *CommandLine::exec(cmd.c_str());
    value = stold(out);
}

functionValue ConstantHeadway::getValue() {
    return value;
}
