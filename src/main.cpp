#include "controller/Controller.h"

#include <iostream>
#include <csignal>

unique_ptr<Controller> ctr;

void interruptHandler([[maybe_unused]] int s) {
    signal(SIGINT, SIG_DFL);
    raise(SIGINT);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        throw invalid_argument("You need to specify a path to the config.json file as argument!");
    }
    ctr = make_unique<Controller>(argv[1]);

    struct sigaction intHandler{};
    intHandler.sa_handler = interruptHandler;
    sigemptyset(&intHandler.sa_mask);
    intHandler.sa_flags = 0;

    sigaction(SIGINT, &intHandler, nullptr);

    ctr->run();

    return 0;
}
