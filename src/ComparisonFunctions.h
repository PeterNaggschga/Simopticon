#ifndef SIMOPTICON_COMPARISONFUNCTIONS_H
#define SIMOPTICON_COMPARISONFUNCTIONS_H

#include "parameters/Parameter.h"

#include <vector>

/**
 * In this file comparison functions are defined which should be used across the whole framework.
 * They can be used to order elements in STL containers.
 */

/**
 * This struct implements the comparison of two vectors of Parameter references.
 */
struct CmpVectorSharedParameter {
    /**
     * Compares two vectors of Parameter references.
     * @param a: First vector to be compared.
     * @param b: Second vector to be compared.
     * @return True if a is smaller in size than b or if a is to be sorted before b by ascending order of coordinates.
     */
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

/**
 * This struct implements the comparison of two pointers to function values.
 */
struct CmpPtrFunctionvalue {
    /**
     * Compares two pointers to function values.
     * @param a: First pointer to a function value.
     * @param b: Second pointer to a function value.
     * @return Compares *a and *b. If *a == *b the addresses are compared.
     */
    bool operator()(const functionValue *a, const functionValue *b) const { return *a == *b ? a < b : *a < *b; }
};

/**
 * This struct implements the comparison of two pairs of Parameter combination and function value.
 */
struct CmpPairVectorSharedParameterFunctionvalue {
    /**
     * Compares two pairs of Parameter combination and function value.
     * @param a: First pair.
     * @param b: Second pair.
     * @return Compares the function values. If they are the same, the Parameter combinations are compared.
     */
    bool operator()(const pair<vector<shared_ptr<Parameter>>, functionValue> &a,
                    const pair<vector<shared_ptr<Parameter>>, functionValue> &b) const {
        return a.second == b.second ? a.first < b.first : a.second < b.second;
    }
};

#endif //SIMOPTICON_COMPARISONFUNCTIONS_H
