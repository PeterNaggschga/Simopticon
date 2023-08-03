#include "controller/Controller.h"
#include "controller/StubController.h"

#include <iostream>
#include <csignal>

/**
 * Definition of the main function running the *Simopticon* framework.
 *
 * @file main.cpp
 * @ingroup utils
 */

/**
 * Reference to the Controller that is running the optimization.
 */
unique_ptr<Controller> ctr;

/**
 * Handler routine for SIGINT signal which calls Controller::abort and sets the new handler of SIGINT to the default (instant interrupt of the software).
 * @param s: Necessary parameter for interrupt handlers (unused).
 * @todo Make interrupt handling independent from OS - currently only Systems using POSIX signals are supported.
 */
void interruptHandler([[maybe_unused]] int s) {
    signal(SIGINT, SIG_DFL);
    ctr->abort();
}

/**
 * Checks correct command line input and registers interrupt handler for SIGINT signal.
 * Instantiates Controller or StubController and kicks of the optimization using Controller::run.
 * @param argc: Number of command line arguments.
 * @param argv: Array of command line arguments.
 * @return Status code.
 */
int main(int argc, char **argv) {
    if (argc < 2) {
        throw invalid_argument("You need to specify a path to the config.json file as argument!");
    }

    if (argc == 2) {
        ctr = make_unique<Controller>(argv[1]);
    } else {
        ctr = make_unique<StubController>(argv[1], argv[2]);
    }

    struct sigaction intHandler{};
    intHandler.sa_handler = interruptHandler;
    sigemptyset(&intHandler.sa_mask);
    intHandler.sa_flags = 0;
    sigaction(SIGINT, &intHandler, nullptr);

    ctr->run();
    return 0;
}
