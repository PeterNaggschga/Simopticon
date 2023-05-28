#include "Optimizer.h"

#include <utility>

map<vector<Parameter>, double> Optimizer::getValues(const list<vector<Parameter>> &params) {
    return controller.getValues(params);
}
