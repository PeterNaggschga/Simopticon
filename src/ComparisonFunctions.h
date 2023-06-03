#ifndef SIMOPTICON_COMPARISONFUNCTIONS_H
#define SIMOPTICON_COMPARISONFUNCTIONS_H

#include "parameters/Parameter.h"

#include <vector>

struct CmpVectorSharedParameter {
    bool operator()(vector<shared_ptr<Parameter>> a, vector<shared_ptr<Parameter>> b) const {
        if (a.size() != b.size()) {
            return a.size() < b.size();
        }
        for (int i = 0; i < a.size(); ++i) {
            if (*a[i] != *b[i]) {
                return *a[i] < *b[i];
            }
        }
        return false;
    }
};

struct CmpPtrFunctionvalue {
    bool operator()(const functionValue *a, const functionValue *b) const { return *a == *b ? a < b : *a < *b; }
};

struct CmpPairVectorSharedParameterFunctionvalue {
    bool operator()(const pair<vector<shared_ptr<Parameter>>, functionValue> &a,
                    const pair<vector<shared_ptr<Parameter>>, functionValue> &b) const {
        return a.second == b.second ? a.first < b.first : a.second < b.second;
    }
};

#endif //SIMOPTICON_COMPARISONFUNCTIONS_H
