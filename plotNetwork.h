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
    //std::unordered_map<int, PlotNode*> id2node;
    std::unordered_map<int, std::shared_ptr<PlotNode>> id2node;
    int plotsPerSide;
public:
    PlotNetwork();

    bool hasNode(int id) const;

    void addNode(PlotNode n);

    // Returns the shortest path between the nodes given by idStart and idEnd in
    // reverse order. If no path exists, returns the path to a random node as far
    // away as possible. If maxDepth is given to be a positive integer, then it will
    // return whatever Plot that is maxDepth from Start is closest to End.
    std::vector<PlotNode*> breadthFirstSearch(int idStart, int idEnd, int maxDepth=-1) const;

    // Wrapper function
    std::vector<PlotNode*> getShortestPath(int idStart, int idEnd, int maxDepth=-1) const;

    // Wrapper function that returns Points (the centers of the Plots), and leaves
    // the order reversed since the Enemies want that
    std::vector<Point> getShortestPathPoints(int idStart, int idEnd, int maxDepth=-1) const;
};

#endif //PRIME_CITY_PLOTNETWORK_H
