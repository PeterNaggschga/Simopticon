/**
 * In this file, the main function running the *Simopticon* framework is defined.
 *
 * @file main.cpp
 * @ingroup utils
 * @author Per Natzschka
 */

/**
 * @dir
 * In this directory, the source code of the *Simopticon* framework is located.
 */

#include "controller/Controller.h"
#include "controller/StubController.h"

#include <iostream>
#include <csignal>

/**
 * Reference to the Controller that is running the optimization.
 */
std::unique_ptr<Controller> ctr;

/**
 * Handler routine for SIGINT signal which calls Controller#abort and sets the new handler of SIGINT to the default (instant interrupt of the software).
 * @param s: Necessary parameter for interrupt handlers (unused).
 */
void interruptHandler([[maybe_unused]] int s) {
    /**
     * @todo Make interrupt handling independent from OS - currently only Systems using POSIX signals are supported.
     */
    signal(SIGINT, SIG_DFL);
    ctr->abort();
}

/**
 * Checks correct command line input and registers interrupt handler for SIGINT signal.
 * Instantiates Controller or StubController and kicks of the optimization using Controller#run.
 * @param argc: Number of command line arguments.
 * @param argv: Array of command line arguments.
 * @return %Status code.
 */
int main(int argc, char **argv) {
    if (argc < 2) {
        throw std::invalid_argument("You need to specify a path to the config.json file as argument!");
    }

    if (argc == 2) {
        ctr = std::make_unique<Controller>(argv[1]);
    } else {
        ctr = std::make_unique<StubController>(argv[1], argv[2]);
    }

    struct sigaction intHandler{};
    intHandler.sa_handler = interruptHandler;
    sigemptyset(&intHandler.sa_mask);
    intHandler.sa_flags = 0;
    sigaction(SIGINT, &intHandler, nullptr);

    ctr->run();
    return 0;
}
