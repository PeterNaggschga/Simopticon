#ifndef SIMOPTICON_PARAMETERNORMALIZER_H
#define SIMOPTICON_PARAMETERNORMALIZER_H


#include "DirectTypes.h"
#include "../../Types.h"

#include <list>
#include <vector>
#include <memory>

class ParameterDefinition;

class Parameter;

using namespace std;

class ParameterNormalizer {
private:
    list<ParameterDefinition> &parameters;

public:
    explicit ParameterNormalizer(list<ParameterDefinition> &parameters);

    static vector<dirCoordinate> normalize(const vector<shared_ptr<Parameter>> &params);

    vector<shared_ptr<Parameter>> denormalize(vector<dirCoordinate> cords);
};


#endif //SIMOPTICON_PARAMETERNORMALIZER_H
