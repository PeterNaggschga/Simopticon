#ifndef SIMOPTICON_PIPELINE_H
#define SIMOPTICON_PIPELINE_H

#include "../Types.h"

#include <string>
#include <list>
#include <set>
#include <filesystem>

using namespace std;

class Pipeline {
public:
    virtual ~Pipeline() = default;

    virtual functionValue processOutput(set<runId> experimentIds, filesystem::path path, unsigned int pipelineId) = 0;

};


#endif //SIMOPTICON_PIPELINE_H
