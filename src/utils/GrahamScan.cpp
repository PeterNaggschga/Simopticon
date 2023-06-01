#include "GrahamScan.h"

#include "../optimizer/direct/hyrect/HyRect.h"

#include <map>

list<pair<shared_ptr<HyRect>, double>> GrahamScan::scan(list<shared_ptr<HyRect>> vertices) {
    map<double, shared_ptr<HyRect>> potentialHull;
    auto it = vertices.begin();
    shared_ptr<HyRect> firstRect = *it;
    potentialHull.insert(make_pair(0, firstRect));
    it++;
    while (it != vertices.end()) {
        auto angle = (double) ((it->get()->getAvgValue() - firstRect->getAvgValue()) /
                               (it->get()->getDiagonalLength() - firstRect->getDiagonalLength()));
        if (angle <= 0) {
            potentialHull.clear();
            firstRect = *it;
            potentialHull.insert(make_pair(0, firstRect));
        } else {
            auto angleSet = potentialHull.find(angle);
            if (angleSet == potentialHull.end()) {
                potentialHull.insert(make_pair(angle, *it));
            } else {
                angleSet->second = *it;
            }
        }
        it++;
    }

    list<pair<shared_ptr<HyRect>, double>> result;
    shared_ptr<HyRect> lastRect = potentialHull.begin()->second;
    for (const pair<const double, shared_ptr<HyRect>> &entry: potentialHull) {
        if (entry == *potentialHull.begin()) {
            continue;
        }
        if (entry.second->getDepth() < lastRect->getDepth()) {
            auto k = (double) ((entry.second->getAvgValue() - lastRect->getAvgValue()) /
                               (entry.second->getDiagonalLength() - lastRect->getDiagonalLength()));
            result.emplace_back(lastRect, k);
            lastRect = entry.second;
        }
    }
    result.emplace_back(lastRect, INFINITY);
    return result;
}
