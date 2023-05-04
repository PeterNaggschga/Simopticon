#ifndef SIMOPTICON_PARAMETERDEFINITION_H
#define SIMOPTICON_PARAMETERDEFINITION_H

#include <string>

using namespace std;

class ParameterDefinition {
private:
    double min;
    double max;
    string unit;
    string config;

public:
    ParameterDefinition(double min, double max, const string& config, const string& unit="");

    double getMin() const;

    double getMax() const;

    const string &getUnit() const;

    const string &getConfig() const;
};


#endif //SIMOPTICON_PARAMETERDEFINITION_H
