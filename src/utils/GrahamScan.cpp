#include "GrahamScan.h"

#include "../optimizer/direct/HyRect.h"

#include <map>

list<pair<HyRect, double>> GrahamScan::scan(list<HyRect> vertices) {
    map<double, HyRect> potentialHull;
    auto it = vertices.begin();
    HyRect firstRect = *it;
    potentialHull.insert(make_pair(0, firstRect));
    it++;
    while (it != vertices.end()) {
        auto angle = (double) ((it->getAvgValue() - firstRect.getAvgValue()) /
                               (it->getDiagonalLength() - firstRect.getDiagonalLength()));
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

    list<pair<HyRect, double>> result;
    HyRect lastRect = potentialHull.begin()->second;
    for (pair<const double, HyRect> entry: potentialHull) {
        if (entry == *potentialHull.begin()) {
            continue;
        }
        if (entry.second.getDepth() < lastRect.getDepth()) {
            auto k = (double) ((entry.second.getAvgValue() - lastRect.getAvgValue()) /
                               (entry.second.getDiagonalLength() - lastRect.getDiagonalLength()));
            result.emplace_back(lastRect, k);
            lastRect = entry.second;
        }
    }
    result.emplace_back(lastRect, INFINITY);
    return result;
}
