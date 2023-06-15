#include "controller/Controller.h"

#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        throw invalid_argument("You need to specify a path to the config.json file as argument!");
    }

    unique_ptr<Controller> ctr(new Controller(argv[1]));
    ctr->run();

    return 0;
}
