#ifndef SIMOPTICON_CHILDRECT_H
#define SIMOPTICON_CHILDRECT_H


#include "HyRect.h"

class ChildRect : public HyRect {
private:
    shared_ptr<HyRect> parent;

public:
    ChildRect(dimension D, position pos, shared_ptr<HyRect> parent);

    array<vector<dirCoordinate>, 2> getSamplingVertices() override;

    bool operator==(const HyRect &rect) const override;
};


#endif //SIMOPTICON_CHILDRECT_H
