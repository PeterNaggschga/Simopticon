#ifndef SIMOPTICON_PIPELINE_H
#define SIMOPTICON_PIPELINE_H

#include "../Types.h"
#include "../status/Status.h"

#include <set>
#include <filesystem>
#include <map>

using namespace std;

class Pipeline : public Status {
public:
    virtual ~Pipeline() = default;

    virtual functionValue processOutput(filesystem::path path, set<runId> experimentIds) = 0;

    virtual map<pair<filesystem::path, set<runId>>, functionValue>
    processOutput(const set<pair<filesystem::path, set<runId>>> &experimentResults);

    string getName() override;

    string getStatus() override;

    string getStatusBar() override;

};


#endif //SIMOPTICON_PIPELINE_H
