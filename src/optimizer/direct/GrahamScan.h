#ifndef SIMOPTICON_GRAHAMSCAN_H
#define SIMOPTICON_GRAHAMSCAN_H

#include <list>
#include <memory>

class HyRect;

using namespace std;

class GrahamScan {

public:
    static list<pair<shared_ptr<HyRect>, double>> scan(list<shared_ptr<HyRect>> vertices);
};


#endif //SIMOPTICON_GRAHAMSCAN_H
