/**
 * @file
 * In this file, the implementation of the GrahamScan class is defined.
 * @author Per Natzschka
 */

#include "GrahamScan.h"

#include "hyrect/HyRect.h"

#include <map>

std::list<std::pair<std::shared_ptr<HyRect>, double>> GrahamScan::scan(std::list<std::shared_ptr<HyRect>> vertices) {
    std::map<double, std::shared_ptr<HyRect>> potentialHull;
    auto it = vertices.begin();
    std::shared_ptr<HyRect> firstRect = vertices.front();
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

    std::list<std::pair<std::shared_ptr<HyRect>, double>> result;
    std::shared_ptr<HyRect> lastRect = potentialHull.begin()->second;
    for (const std::pair<const double, std::shared_ptr<HyRect>> &entry: potentialHull) {
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
