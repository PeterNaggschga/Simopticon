#ifndef SIMOPTICON_COMMANDLINE_H
#define SIMOPTICON_COMMANDLINE_H

#include <string>
#include <memory>

using namespace std;

/**
 * A class containing functionality for executing commands on UNIX shell.
 */
class CommandLine {
public:
    /**
     * Executes the given command in UNIX shell and returns the output (both stderr and stdout merged).
     * @param cmd: Command to be executed.
     * @return A string containing the output (sterr and stdout merged).
     */
    static unique_ptr<string> exec(string cmd);

};

#endif //SIMOPTICON_COMMANDLINE_H
