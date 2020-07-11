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
    int chunkSize;
    int plotSize;
public:
    PlotNetwork(int inputChunkSize, int inputPlotsPerSide);

    bool hasNode(int id) const;

    void addNode(PlotNode n);

    // Returns the shortest path between the nodes given by idStart and idEnd in
    // reverse order. If no path exists, returns the path to a random node as far
    // away as possible. If maxDepth is given to be a positive integer, then it will
    // return whatever Plot that is maxDepth from Start is closest to End.
    std::vector<int> breadthFirstSearch(int idStart, int idEnd, int maxDepth=-1) const;

    // Wrapper function
    std::vector<PlotNode*> getShortestPath(int idStart, int idEnd, int maxDepth=-1) const;

    // Wrapper function that returns Points (the centers of the Plots), and leaves
    // the order reversed since the Enemies want that
    std::vector<Point> getShortestPathPoints(int idStart, int idEnd, int maxDepth=-1) const;

    // Returns a vector of the IDs of the plots intersected by the line between the
    // two input plots. This considers all lines between points in the two plots,
    // not just the centers.
    std::vector<int> getPlotIDsBetween(int plotID1, int plotID2) const;

    // Returns true if every point in the given plots has an open line to
    // every point in the other plot
    bool hasLineOfSight(int plotID1, int plotID2) const;

    // Returns a shortened version of path if is possible to cut across diagonals
    // and still stay in valid nodes in the network
    std::vector<int> clipPath(std::vector<int> path);
};

#endif //PRIME_CITY_PLOTNETWORK_H
