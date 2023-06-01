#ifndef SIMOPTICON_BASERECT_H
#define SIMOPTICON_BASERECT_H


#include "HyRect.h"

class BaseRect : public HyRect {
public:
    explicit BaseRect(dimension D);

    array<vector<dirCoordinate>, 2> getSamplingVertices() override;
};


#endif //SIMOPTICON_BASERECT_H
