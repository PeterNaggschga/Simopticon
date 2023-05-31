#ifndef SIMOPTICON_GRAHAMSCAN_H
#define SIMOPTICON_GRAHAMSCAN_H

#include <list>

class HyRect;

using namespace std;

class GrahamScan {

public:
    static list<pair<HyRect, double>> scan(list<HyRect> vertices);
};


#endif //SIMOPTICON_GRAHAMSCAN_H
