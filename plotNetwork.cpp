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
    id2node[n.getID()] = std::make_shared<PlotNode>(n);

    // Add pointers between the adjacent nodes, if they exist
    int plotIDAbove = idAbove(n.getID(), plotsPerSide);
    if(id2node.count(plotIDAbove) != 0)
    {
        id2node[n.getID()]->addUpNeighbor(*id2node[plotIDAbove]);
        id2node[plotIDAbove]->addDownNeighbor(*id2node[n.getID()]);
    }
    int plotIDBelow = idBelow(n.getID(), plotsPerSide);
    if(id2node.count(plotIDBelow) != 0)
    {
        id2node[n.getID()]->addDownNeighbor(*id2node[plotIDBelow]);
        id2node[plotIDBelow]->addUpNeighbor(*id2node[n.getID()]);
    }
    int plotIDLeft = idLeft(n.getID(), plotsPerSide);
    if(id2node.count(plotIDLeft) != 0)
    {
        id2node[n.getID()]->addLeftNeighbor(*id2node[plotIDLeft]);
        id2node[plotIDLeft]->addRightNeighbor(*id2node[n.getID()]);
    }
    int plotIDRight = idRight(n.getID(), plotsPerSide);
    if(id2node.count(plotIDRight) != 0)
    {
        id2node[n.getID()]->addRightNeighbor(*id2node[plotIDRight]);
        id2node[plotIDRight]->addLeftNeighbor(*id2node[n.getID()]);
    }
}

// Returns the shortest path between the nodes given by idStart and idEnd in
// reverse order
std::vector<PlotNode*> PlotNetwork::breadthFirstSearch(int idStart, int idEnd, int maxDepth) const
{
    std::vector<PlotNode*> output;

    // Keep track of two layers of nodes at each step by storing
    // their plotID's
    std::vector<int> prevLevel;
    std::vector<int> curLevel;

    // Map each node visited to the previous node that was used
    // to get to it
    std::unordered_map<int, int> node2prev;

    curLevel.push_back(idStart);
    bool hasBeenFound = false;
    int curDepth = 0;

    while(!hasBeenFound && !curLevel.empty() && curDepth != maxDepth)
    {
        curDepth++;
        prevLevel = curLevel;
        curLevel = std::vector<int>();
        // Iterate through the previous level and look at each neighbor
        for(int plotID : prevLevel)
        {
            // Check the neighbor above this plot
            if(id2node.at(plotID)->hasUpNeighbor())
            {
                int upNeighborID = idAbove(plotID, plotsPerSide);
                if(node2prev.count(upNeighborID) == 0)
                {
                     node2prev[upNeighborID] = plotID;
                     curLevel.push_back(upNeighborID);
                     if(upNeighborID == idEnd)
                     {
                         hasBeenFound = true;
                         break;
                     }
                }
            }
            // Check the neighbor below this plot
            if(id2node.at(plotID)->hasDownNeighbor())
            {
                int downNeighborID = idBelow(plotID, plotsPerSide);
                if(node2prev.count(downNeighborID) == 0)
                {
                    node2prev[downNeighborID] = plotID;
                    curLevel.push_back(downNeighborID);
                    if(downNeighborID == idEnd)
                    {
                        hasBeenFound = true;
                        break;
                    }
                }
            }
            // Check the neighbor left of this plot
            if(id2node.at(plotID)->hasLeftNeighbor())
            {
                int leftNeighborID = idLeft(plotID, plotsPerSide);
                if(node2prev.count(leftNeighborID) == 0)
                {
                    node2prev[leftNeighborID] = plotID;
                    curLevel.push_back(leftNeighborID);
                    if(leftNeighborID == idEnd)
                    {
                        hasBeenFound = true;
                        break;
                    }
                }
            }
            // Check the neighbor right of this plot
            if(id2node.count(plotID) == 0)
            {
                int a = 0;
            }
            if(id2node.at(plotID)->hasRightNeighbor())
            {
                int rightNeighborID = idRight(plotID, plotsPerSide);
                if(node2prev.count(rightNeighborID) == 0)
                {
                    node2prev[rightNeighborID] = plotID;
                    curLevel.push_back(rightNeighborID);
                    if(rightNeighborID == idEnd)
                    {
                        hasBeenFound = true;
                        break;
                    }
                }
            }
        }
    }

    // Trace backwards and add the path to the output
    int curID;
    if(hasBeenFound)  // If we found the end, trace back from there
    {
        curID = idEnd;
    }
    else if(id2node.count(idEnd) > 0 && curDepth == maxDepth)  // Pick as close to the end as possible
    {
        double shortestDistance = distance2d(id2node.at(idEnd)->getCenter(), id2node.at(prevLevel[0])->getCenter());
        int idOfClosest = prevLevel[0];
        for(int i = 1; i < prevLevel.size(); i++) // iterate through the last level and select the closest
        {
            int curDistance = distance2d(id2node.at(idEnd)->getCenter(), id2node.at(prevLevel[i])->getCenter());
            if(curDistance < shortestDistance)
            {
                shortestDistance = curDistance;
                idOfClosest = prevLevel[i];
            }
        }
        curID = idOfClosest;
    }
    else              // Otherwise, pick a random node in the last level reached
    {
        int randIndex = rand() % prevLevel.size();
        curID = prevLevel[randIndex];
    }

    while(curID != idStart)
    {
        output.push_back(&*id2node.at(curID));
        curID = node2prev[curID];
    }
    output.push_back(&*id2node.at(idStart));
    return output;


}

// Wrapper function
std::vector<PlotNode*> PlotNetwork::getShortestPath(int idStart, int idEnd, int maxDepth) const
{
    std::vector<PlotNode*> output;
    std::vector<PlotNode*> reverseNodes = breadthFirstSearch(idStart, idEnd, maxDepth);
    for(int i = reverseNodes.size() - 1; i >= 0; i--)
    {
        output.push_back(reverseNodes[i]);
    }
    return output;
}

std::vector<Point> PlotNetwork::getShortestPathPoints(int idStart, int idEnd, int maxDepth) const
{
    std::vector<Point> output;
    std::vector<PlotNode*> nodes = breadthFirstSearch(idStart, idEnd, maxDepth);
    for(int i = 0; i < nodes.size(); i++)
    {
        output.push_back(nodes[i]->getCenter());
    }
    return output;
}
