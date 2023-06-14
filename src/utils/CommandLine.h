#ifndef SIMOPTICON_COMMANDLINE_H
#define SIMOPTICON_COMMANDLINE_H

#include <string>
#include <memory>

using namespace std;

class CommandLine {
public:
    static unique_ptr<string> exec(string cmd);

};

#endif //SIMOPTICON_COMMANDLINE_H
