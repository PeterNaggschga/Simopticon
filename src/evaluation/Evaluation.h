#ifndef SIMOPTICON_EVALUATION_H
#define SIMOPTICON_EVALUATION_H

#include "../Types.h"
#include "../status/Status.h"

#include <set>
#include <filesystem>
#include <map>

using namespace std;

class Evaluation : public Status {
public:
    virtual ~Evaluation() = default;

    virtual functionValue processOutput(filesystem::path path, set<runId> experimentIds) = 0;

    virtual map<pair<filesystem::path, set<runId>>, functionValue>
    processOutput(const set<pair<filesystem::path, set<runId>>> &experimentResults);

    string getName() override;

    string getStatus() override;

    string getStatusBar() override;

};


#endif //SIMOPTICON_EVALUATION_H
