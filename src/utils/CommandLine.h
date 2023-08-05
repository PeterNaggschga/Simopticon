#ifndef SIMOPTICON_COMMANDLINE_H
#define SIMOPTICON_COMMANDLINE_H

#include <string>
#include <memory>

using namespace std;

/**
 * This module provides general functionality and classes that may be useful to classes in any other package.
 * @defgroup utils utils
 */

/**
 * A class containing functionality for executing commands on UNIX shell.
 * @ingroup utils
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
