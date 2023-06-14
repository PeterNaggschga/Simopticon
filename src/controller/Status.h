#ifndef SIMOPTICON_STATUS_H
#define SIMOPTICON_STATUS_H

#include <string>

using namespace std;

class Status {
public:
    const static string NO_STATUS_SUPPORT;

    const static string NO_NAME;

    [[nodiscard]] virtual string getName();

    [[nodiscard]] virtual string getStatus();

    [[nodiscard]] virtual string getStatusBar();
};

#endif //SIMOPTICON_STATUS_H
