#include "plotNetwork.h"

PlotNetwork::PlotNetwork()
{
    plotsPerSide = 8;
}

bool PlotNetwork::hasNode(int id) const
{
    return id2node.count(id) > 0;
}

void PlotNetwork::addNode(PlotNode n)
{
    // Put the node in the map
    id2node[n.getID()] = &n;

    // Add pointers between the adjacent nodes, if they exist
    int plotIDAbove = idAbove(n.getID(), plotsPerSide);
    if(id2node.count(plotIDAbove) != 0)
    {
        id2node[n.getID()]->addUpNeighbor(id2node[plotIDAbove]);
        id2node[plotIDAbove]->addDownNeighbor(id2node[n.getID()]);
    }
    int plotIDBelow = idBelow(n.getID(), plotsPerSide);
    if(id2node.count(plotIDBelow) != 0)
    {
        id2node[n.getID()]->addDownNeighbor(id2node[plotIDBelow]);
        id2node[plotIDBelow]->addUpNeighbor(id2node[n.getID()]);
    }
    int plotIDLeft = idLeft(n.getID(), plotsPerSide);
    if(id2node.count(plotIDLeft) != 0)
    {
        id2node[n.getID()]->addLeftNeighbor(id2node[plotIDLeft]);
        id2node[plotIDLeft]->addRightNeighbor(id2node[n.getID()]);
    }
    int plotIDRight = idRight(n.getID(), plotsPerSide);
    if(id2node.count(plotIDRight) != 0)
    {
        id2node[n.getID()]->addRightNeighbor(id2node[plotIDRight]);
        id2node[plotIDRight]->addLeftNeighbor(id2node[n.getID()]);
    }
}

// Returns the shortest path between the nodes given by idStart and idEnd in
// reverse order
std::vector<PlotNode*> PlotNetwork::breadthFirstSearch(int idStart, int idEnd) const
{
    return std::vector<PlotNode*>();
}

// Wrapper function
std::vector<PlotNode*> PlotNetwork::getShortestPath(int idStart, int idEnd) const
{
    return std::vector<PlotNode*>();
}