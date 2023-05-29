#ifndef SIMOPTICON_PARAMETER_H
#define SIMOPTICON_PARAMETER_H


#include <string>

class ParameterDefinition;

using namespace std;

class Parameter {
private:
    ParameterDefinition &definition;

public:
    explicit Parameter(ParameterDefinition &def);

    [[nodiscard]] double getMin() const;

    [[nodiscard]] double getMax() const;

    [[nodiscard]] const string &getUnit() const;

    [[nodiscard]] const string &getConfig() const;

    [[nodiscard]] virtual double getVal() const = 0;

    virtual void setVal(double val) = 0;
};

#endif //SIMOPTICON_PARAMETER_H
