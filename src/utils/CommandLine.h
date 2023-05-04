#ifndef SIMOPTICON_COMMANDLINE_H
#define SIMOPTICON_COMMANDLINE_H

#include <string>

using namespace std;

class CommandLine {
public:
    static const string SOURCE;

    static string exec(const char *cmd);
};

#endif //SIMOPTICON_COMMANDLINE_H
