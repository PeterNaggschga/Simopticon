#ifndef SIMOPTICON_PARAMETERNORMALIZER_H
#define SIMOPTICON_PARAMETERNORMALIZER_H


#include <list>
#include <vector>
#include "DirectTypes.h"
#include "../../Types.h"

class ParameterDefinition;

class Parameter;

using namespace std;

class ParameterNormalizer {
private:
    list<ParameterDefinition> &parameters;

public:
    explicit ParameterNormalizer(list<ParameterDefinition> &parameters);

    static vector<dirCoordinate> normalize(const vector<Parameter *> &params);

    vector<Parameter *> denormalize(vector<dirCoordinate> cords);
};


#endif //SIMOPTICON_PARAMETERNORMALIZER_H
