#ifndef SIMOPTICON_PIPELINE_H
#define SIMOPTICON_PIPELINE_H

#include <string>
#include <list>

using namespace std;

class Pipeline {
public:
    virtual void processOutput(list<string> experimentIds, string path, int pipelineId) = 0;

    virtual double getValue() = 0;
};


#endif //SIMOPTICON_PIPELINE_H
