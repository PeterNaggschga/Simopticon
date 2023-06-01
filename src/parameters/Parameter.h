#ifndef SIMOPTICON_PARAMETER_H
#define SIMOPTICON_PARAMETER_H


#include "../Types.h"

#include <string>
#include <memory>

class ParameterDefinition;

using namespace std;

class Parameter {
private:
    shared_ptr<ParameterDefinition> definition;

public:
    explicit Parameter(shared_ptr<ParameterDefinition> def);

    virtual ~Parameter() = default;

    [[nodiscard]] coordinate getMin() const;

    [[nodiscard]] coordinate getMax() const;

    [[nodiscard]] const string &getUnit() const;

    [[nodiscard]] const string &getConfig() const;

    [[nodiscard]] virtual coordinate getVal() const = 0;

    virtual void setVal(coordinate val) = 0;
};

#endif //SIMOPTICON_PARAMETER_H
