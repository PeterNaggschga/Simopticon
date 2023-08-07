/**
 * @file
 * In this file, the implementation of the GrahamScan class is defined.
 */

#include "GrahamScan.h"

#include "hyrect/HyRect.h"

#include <map>

list<pair<shared_ptr<HyRect>, double>> GrahamScan::scan(list<shared_ptr<HyRect>> vertices) {
    map<double, shared_ptr<HyRect>> potentialHull;
    auto it = vertices.begin();
    shared_ptr<HyRect> firstRect = vertices.front();
    potentialHull.insert(make_pair(0, firstRect));
    vertices.erase(vertices.begin());
    for (const auto &entry: vertices) {
        auto angle = (double) ((entry->getAvgValue() - firstRect->getAvgValue()) /
                               (entry->getDiagonalLength() - firstRect->getDiagonalLength()));
        if (angle <= 0) {
            potentialHull.clear();
            firstRect = entry;
            angle = 0;
        } else {
            potentialHull.erase(angle);
        }
        potentialHull.insert(make_pair(angle, entry));
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
