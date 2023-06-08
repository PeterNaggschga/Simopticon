#ifndef SIMOPTICON_PIPELINE_H
#define SIMOPTICON_PIPELINE_H

#include "../Types.h"

#include <string>
#include <list>
#include <set>
#include <filesystem>
#include <map>

using namespace std;

class Pipeline {
public:
    virtual ~Pipeline() = default;

    virtual functionValue processOutput(unsigned int pipelineId, filesystem::path path, set<runId> experimentIds) = 0;

    virtual map<tuple<unsigned int, filesystem::path, set<runId>>, functionValue> processOutput(
            const set<tuple<unsigned int, filesystem::path, set<runId>>> &experiments);

};


#endif //SIMOPTICON_PIPELINE_H
