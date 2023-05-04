#include "ConstantHeadway.h"
#include "../utils/CommandLine.h"
#include <string>

using namespace std;

void ConstantHeadway::processOutput(list<string> experimentIds, string path, int pipelineId) {
    string pipeline = CommandLine::SOURCE + "src/evaluation/pipeline/pipeline.py ";
    string cmd = pipeline + "-p " + path;
    string out = CommandLine::exec(cmd.c_str());
    value = stod(out);
}

double ConstantHeadway::getValue() {
    return value;
}
