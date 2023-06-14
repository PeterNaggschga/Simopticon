#ifndef SIMOPTICON_PIPELINE_H
#define SIMOPTICON_PIPELINE_H

#include "../Types.h"
#include "../controller/Status.h"

#include <set>
#include <filesystem>
#include <map>

using namespace std;

class Pipeline : public Status {
public:
    virtual ~Pipeline() = default;

    virtual functionValue processOutput(filesystem::path path, set<runId> experimentIds, unsigned int pipelineId) = 0;

    virtual map<pair<filesystem::path, set<runId>>, functionValue>
    processOutput(const set<pair<filesystem::path, set<runId>>> &experimentResults, unsigned int pipelineId);

};


#endif //SIMOPTICON_PIPELINE_H
