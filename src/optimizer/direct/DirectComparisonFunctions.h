#ifndef SIMOPTICON_DIRECTCOMPARISONFUNCTIONS_H
#define SIMOPTICON_DIRECTCOMPARISONFUNCTIONS_H

#include "hyrect/HyRect.h"

struct CmpSharedHyrect {
    bool operator()(const shared_ptr<HyRect> &a, const shared_ptr<HyRect> &b) const {
        return a->getAvgValue() == b->getAvgValue() ? a->getSamplingVertices() < b->getSamplingVertices() :
               a->getAvgValue() < b->getAvgValue();
    }
};

#endif //SIMOPTICON_DIRECTCOMPARISONFUNCTIONS_H
