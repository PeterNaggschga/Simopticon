/**
 * @file
 * In this file, the implementation of the Evaluation class is defined.
 * @author Per Natzschka
 */

#include "Evaluation.h"

std::map<std::pair<std::filesystem::path, std::set<runId>>, functionValue>
Evaluation::processOutput(const std::set<std::pair<std::filesystem::path, std::set<runId>>> &experimentResults) {
    std::map<std::pair<std::filesystem::path, std::set<runId>>, functionValue> result;
    for (const auto &entry: experimentResults) {
        result.insert(make_pair(entry, processOutput(entry.first, entry.second)));
    }
    return result;
}

std::string Evaluation::getName() {
    return Status::getName();
}

std::string Evaluation::getStatus() {
    return Status::getStatus();
}

std::string Evaluation::getStatusBar() {
    return Status::getStatusBar();
}
