#ifndef SIMOPTICON_PARAMETER_H
#define SIMOPTICON_PARAMETER_H


#include <string>
#include "../Types.h"

class ParameterDefinition;

using namespace std;

class Parameter {
private:
    ParameterDefinition &definition;

public:
    explicit Parameter(ParameterDefinition &def);

    virtual ~Parameter() = default;

    [[nodiscard]] double getMin() const;

    [[nodiscard]] double getMax() const;

    [[nodiscard]] const string &getUnit() const;

    [[nodiscard]] const string &getConfig() const;

    [[nodiscard]] virtual coordinate getVal() const = 0;

    virtual void setVal(coordinate val) = 0;
};

#endif //SIMOPTICON_PARAMETER_H
