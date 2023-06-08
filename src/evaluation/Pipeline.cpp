#include "Pipeline.h"

map<tuple<unsigned int, filesystem::path, set<runId>>, functionValue>
Pipeline::processOutput(const set<tuple<unsigned int, filesystem::path, set<runId>>> &experiments) {
    map<tuple<unsigned int, filesystem::path, set<runId>>, functionValue> result;
    for (auto entry: experiments) {
        result.insert(make_pair(entry, processOutput(get<0>(entry), get<1>(entry), get<2>(entry))));
    }
    return result;
}
