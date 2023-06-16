#include "Pipeline.h"

map<pair<filesystem::path, set<runId>>, functionValue>
Pipeline::processOutput(const set<pair<filesystem::path, set<runId>>> &experimentResults) {
    map<pair<filesystem::path, set<runId>>, functionValue> result;
    for (const auto &entry: experimentResults) {
        result.insert(make_pair(entry, processOutput(entry.first, entry.second)));
    }
    return result;
}

string Pipeline::getName() {
    return Status::getName();
}

string Pipeline::getStatus() {
    return Status::getStatus();
}

string Pipeline::getStatusBar() {
    return Status::getStatusBar();
}
