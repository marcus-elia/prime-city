#ifndef PRIME_CITY_PLOTNETWORK_H
#define PRIME_CITY_PLOTNETWORK_H

#include "plotNode.h"
#include "mathHelper.h"
#include <experimental/optional>
#include <unordered_map>
#include <vector>
#include <memory>

class PlotNetwork
{
private:
    std::unordered_map<int, PlotNode*> id2node;
    int plotsPerSide;
public:
    PlotNetwork();

    bool hasNode(int id) const;

    void addNode(PlotNode n);

    // Returns the shortest path between the nodes given by idStart and idEnd in
    // reverse order
    std::vector<PlotNode*> breadthFirstSearch(int idStart, int idEnd) const;

    // Wrapper function
    std::vector<PlotNode*> getShortestPath(int idStart, int idEnd) const;
};

#endif //PRIME_CITY_PLOTNETWORK_H
