#ifndef SIMOPTICON_PARAMETERDEFINITION_H
#define SIMOPTICON_PARAMETERDEFINITION_H


#include "../Types.h"

#include <string>

using namespace std;

class ParameterDefinition {
private:
    const coordinate min;
    const coordinate max;
    const string unit;
    const string config;

public:
    ParameterDefinition(coordinate min, coordinate max, string config, string unit = "");

    [[nodiscard]] coordinate getMin() const;

    [[nodiscard]] coordinate getMax() const;

    [[nodiscard]] const string &getUnit() const;

    [[nodiscard]] const string &getConfig() const;
};


#endif //SIMOPTICON_PARAMETERDEFINITION_H
