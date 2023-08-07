/**
 * @file
 * In this file, the implementation of the Evaluation class is defined.
 */

#include "Evaluation.h"

map<pair<filesystem::path, set<runId>>, functionValue>
Evaluation::processOutput(const set<pair<filesystem::path, set<runId>>> &experimentResults) {
    map<pair<filesystem::path, set<runId>>, functionValue> result;
    for (const auto &entry: experimentResults) {
        result.insert(make_pair(entry, processOutput(entry.first, entry.second)));
    }
    return result;
}

string Evaluation::getName() {
    return Status::getName();
}

string Evaluation::getStatus() {
    return Status::getStatus();
}

string Evaluation::getStatusBar() {
    return Status::getStatusBar();
}
