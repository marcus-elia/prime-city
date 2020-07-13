#include "plotNetwork.h"

PlotNetwork::PlotNetwork()
{
    chunkSize = 512;
    plotsPerSide = 8;
    plotSize = chunkSize / plotsPerSide;
}
PlotNetwork::PlotNetwork(int inputChunkSize, int inputPlotsPerSide)
{
    chunkSize = inputChunkSize;
    plotsPerSide = inputPlotsPerSide;
    plotSize = chunkSize / plotsPerSide;
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
std::vector<int> PlotNetwork::breadthFirstSearch(int idStart, int idEnd, int maxDepth) const
{
    std::vector<int> output;

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
        //output.push_back(&*id2node.at(curID));
        output.push_back(curID);
        curID = node2prev[curID];
    }
    output.push_back(idStart);
    return output;


}

// Wrapper function
std::vector<PlotNode*> PlotNetwork::getShortestPath(int idStart, int idEnd, int maxDepth) const
{
    std::vector<PlotNode*> output;
    std::vector<int> reverseNodes = breadthFirstSearch(idStart, idEnd, maxDepth);
    for(int i = reverseNodes.size() - 1; i >= 0; i--)
    {
        output.push_back(&*id2node.at(reverseNodes[i]));
    }
    return output;
}

std::vector<Point> PlotNetwork::getShortestPathPoints(int idStart, int idEnd, int maxDepth) const
{
    std::vector<Point> output;
    std::vector<int> nodes = breadthFirstSearch(idStart, idEnd, maxDepth);
    for(int i = 0; i < nodes.size(); i++)
    {
        output.push_back(id2node.at(nodes[i])->getCenter());
    }
    return output;
}

std::vector<Point> PlotNetwork::getClippedPathPoints(int idStart, int idEnd, int maxDepth) const
{
    std::vector<Point> output;
    std::vector<int> nodes = clipPath(breadthFirstSearch(idStart, idEnd, maxDepth));
    for(int i = 0; i < nodes.size(); i++)
    {
        output.push_back(id2node.at(nodes[i])->getCenter());
    }
    return output;
}

std::vector<int> PlotNetwork::getPlotIDsBetween(int plotID1, int plotID2) const
{
    std::vector<int> ids;
    Point startCenter = getPlotCenterFromID(plotID1, chunkSize, plotsPerSide);
    Point endCenter = getPlotCenterFromID(plotID2, chunkSize, plotsPerSide);

    // First, handle the four cases of lines along axes
    if(startCenter.x == endCenter.x || startCenter.z == endCenter.z)
    {
        direction dir;
        // If end is up relative to start
        if(startCenter.z > endCenter.z)
        {
            dir = Up;
        }
        else if(startCenter.z < endCenter.z)
        {
            dir = Down;
        }
        else if(startCenter.x < endCenter.x)
        {
            dir = Right;
        }
        else
        {
            dir = Left;
        }
        //int curPlotID = getIDinDirection(plotID1, plotsPerSide, dir);
        int curPlotID = plotID1;
        while(curPlotID != plotID2)
        {
            ids.push_back(curPlotID);
            curPlotID = getIDinDirection(curPlotID, plotsPerSide, dir);
        }
        ids.push_back(plotID2);
    }

    // Now handle the four cases of diagonal lines
    // End is up and right of start
    else if(endCenter.x > startCenter.x && endCenter.z < startCenter.z)
    {
        // We need to know the two lines from the opposite corners of the
        // start plot to the end plot (the two slopes are probably the same)
        // Higher line
        Point startTopLeft = getPlotTopLeftFromID(plotID1, chunkSize, plotsPerSide);
        Point endTopLeft = getPlotTopLeftFromID(plotID2, chunkSize, plotsPerSide);
        double higherSlope = (endTopLeft.z - startTopLeft.z) / (endTopLeft.x - startTopLeft.x);
        double higherZint = startTopLeft.z - higherSlope*startTopLeft.x;
        // Lower line
        Point startBottomRight = getPlotBottomRightFromID(plotID1, chunkSize, plotsPerSide);
        Point endBottomRight = getPlotBottomRightFromID(plotID2, chunkSize, plotsPerSide);
        double lowerSlope = (endBottomRight.z - startBottomRight.z) / (endBottomRight.x - startBottomRight.x);
        double lowerZint = startBottomRight.z - lowerSlope*startBottomRight.x;

        int curPlotID = plotID1;          // The plot we are considering adding
        int bottomOfColumnID = plotID1;   // Keep track of the bottom of each column also
        Point curTopLeft = getPlotTopLeftFromID(curPlotID, chunkSize, plotsPerSide);
        Point curBottomRight = getPlotBottomRightFromID(curPlotID, chunkSize, plotsPerSide);
        while(curBottomRight.x <= endBottomRight.x) // From left to right, go up the columns
        {
            // Go up while the bottom of the plot is low enough to possibly be in the line
            while(!pointStrictlyAboveLine(curBottomRight, higherSlope, higherZint, -0.1))
            {
                // If we are at the destination, add it to the list and stop looking
                if(curPlotID == plotID2)
                {
                    ids.push_back(curPlotID);
                    break;
                }
                // If the top of the plot is high enough, then it is on the line
                if(pointStrictlyAboveLine(curTopLeft, lowerSlope, lowerZint, 0.1))
                {
                    ids.push_back(curPlotID);
                }
                // Now look at the next plot up vertically
                curPlotID = idAbove(curPlotID, plotsPerSide);
                curTopLeft = getPlotTopLeftFromID(curPlotID, chunkSize, plotsPerSide);
                curBottomRight = getPlotBottomRightFromID(curPlotID, chunkSize, plotsPerSide);
            }
            // Move to the next column of plots
            bottomOfColumnID = idRight(bottomOfColumnID, plotsPerSide);
            curPlotID = bottomOfColumnID;
            curTopLeft = getPlotTopLeftFromID(curPlotID, chunkSize, plotsPerSide);
            curBottomRight = getPlotBottomRightFromID(curPlotID, chunkSize, plotsPerSide);
        }
    }
    // End is up and left of start
    else if(endCenter.x < startCenter.x && endCenter.z < startCenter.z)
    {
        // We need to know the two lines from the opposite corners of the
        // start plot to the end plot (the two slopes are probably the same)
        // Higher line
        Point startTopRight = getPlotTopRightFromID(plotID1, chunkSize, plotsPerSide);
        Point endTopRight = getPlotTopRightFromID(plotID2, chunkSize, plotsPerSide);
        double higherSlope = (endTopRight.z - startTopRight.z) / (endTopRight.x - startTopRight.x);
        double higherZint = startTopRight.z - higherSlope*startTopRight.x;
        // Lower line
        Point startBottomLeft = getPlotBottomLeftFromID(plotID1, chunkSize, plotsPerSide);
        Point endBottomLeft = getPlotBottomLeftFromID(plotID2, chunkSize, plotsPerSide);
        double lowerSlope = (endBottomLeft.z - startBottomLeft.z) / (endBottomLeft.x - startBottomLeft.x);
        double lowerZint = startBottomLeft.z - lowerSlope*startBottomLeft.x;

        int curPlotID = plotID1;          // The plot we are considering adding
        int bottomOfColumnID = plotID1;   // Keep track of the bottom of each column also
        Point curTopRight = getPlotTopRightFromID(curPlotID, chunkSize, plotsPerSide);
        Point curBottomLeft = getPlotBottomLeftFromID(curPlotID, chunkSize, plotsPerSide);
        while(curBottomLeft.x >= endBottomLeft.x) // From right to left, go up the columns
        {
            // Go up while the bottom of the plot is low enough to possibly be in the line
            while(!pointStrictlyAboveLine(curBottomLeft, higherSlope, higherZint, -0.1))
            {
                // If we are at the destination, add it to the list and stop looking
                if(curPlotID == plotID2)
                {
                    ids.push_back(curPlotID);
                    break;
                }
                // If the top of the plot is high enough, then it is on the line
                if(pointStrictlyAboveLine(curTopRight, lowerSlope, lowerZint, 0.1))
                {
                    ids.push_back(curPlotID);
                }
                // Now look at the next plot up vertically
                curPlotID = idAbove(curPlotID, plotsPerSide);
                curTopRight = getPlotTopRightFromID(curPlotID, chunkSize, plotsPerSide);
                curBottomLeft = getPlotBottomLeftFromID(curPlotID, chunkSize, plotsPerSide);
            }
            // Move to the next column of plots
            bottomOfColumnID = idLeft(bottomOfColumnID, plotsPerSide);
            curPlotID = bottomOfColumnID;
            curTopRight = getPlotTopRightFromID(curPlotID, chunkSize, plotsPerSide);
            curBottomLeft = getPlotBottomLeftFromID(curPlotID, chunkSize, plotsPerSide);
        }
    }
    // End is down and left of start
    else if(endCenter.x < startCenter.x && endCenter.z > startCenter.z)
    {
        // We need to know the two lines from the opposite corners of the
        // start plot to the end plot (the two slopes are probably the same)
        // Higher line
        Point startTopLeft = getPlotTopLeftFromID(plotID1, chunkSize, plotsPerSide);
        Point endTopLeft = getPlotTopLeftFromID(plotID2, chunkSize, plotsPerSide);
        double higherSlope = (endTopLeft.z - startTopLeft.z) / (endTopLeft.x - startTopLeft.x);
        double higherZint = startTopLeft.z - higherSlope*startTopLeft.x;
        // Lower line
        Point startBottomRight = getPlotBottomRightFromID(plotID1, chunkSize, plotsPerSide);
        Point endBottomRight = getPlotBottomRightFromID(plotID2, chunkSize, plotsPerSide);
        double lowerSlope = (endBottomRight.z - startBottomRight.z) / (endBottomRight.x - startBottomRight.x);
        double lowerZint = startBottomRight.z - lowerSlope*startBottomRight.x;

        int curPlotID = plotID1;          // The plot we are considering adding
        int topOfColumnID = plotID1;      // Keep track of the top of each column also
        Point curTopLeft = getPlotTopLeftFromID(curPlotID, chunkSize, plotsPerSide);
        Point curBottomRight = getPlotBottomRightFromID(curPlotID, chunkSize, plotsPerSide);
        while(curTopLeft.x >= endTopLeft.x) // From right to left, go down the columns
        {
            // Go down while the top of the plot is high enough to possibly be in the line
            while(pointStrictlyAboveLine(curTopLeft, lowerSlope, lowerZint, 0.1))
            {
                // If we are at the destination, add it to the list and stop looking
                if(curPlotID == plotID2)
                {
                    ids.push_back(curPlotID);
                    break;
                }
                // If the bottom of the plot is low enough, then it is on the line
                if(!pointWeaklyAboveLine(curBottomRight, higherSlope, higherZint))
                {
                    ids.push_back(curPlotID);
                }
                // Now look at the next plot down vertically
                curPlotID = idBelow(curPlotID, plotsPerSide);
                curTopLeft = getPlotTopLeftFromID(curPlotID, chunkSize, plotsPerSide);
                curBottomRight = getPlotBottomRightFromID(curPlotID, chunkSize, plotsPerSide);
            }
            // Move to the next column of plots
            topOfColumnID = idLeft(topOfColumnID, plotsPerSide);
            curPlotID = topOfColumnID;
            curTopLeft = getPlotTopLeftFromID(curPlotID, chunkSize, plotsPerSide);
            curBottomRight = getPlotBottomRightFromID(curPlotID, chunkSize, plotsPerSide);
        }
    }
    // End is down and right of start
    else
    {
        // We need to know the two lines from the opposite corners of the
        // start plot to the end plot (the two slopes are probably the same)
        // Higher line
        Point startTopRight = getPlotTopRightFromID(plotID1, chunkSize, plotsPerSide);
        Point endTopRight = getPlotTopRightFromID(plotID2, chunkSize, plotsPerSide);
        double higherSlope = (endTopRight.z - startTopRight.z) / (endTopRight.x - startTopRight.x);
        double higherZint = startTopRight.z - higherSlope*startTopRight.x;
        // Lower line
        Point startBottomLeft = getPlotBottomLeftFromID(plotID1, chunkSize, plotsPerSide);
        Point endBottomLeft = getPlotBottomLeftFromID(plotID2, chunkSize, plotsPerSide);
        double lowerSlope = (endBottomLeft.z - startBottomLeft.z) / (endBottomLeft.x - startBottomLeft.x);
        double lowerZint = startBottomLeft.z - lowerSlope*startBottomLeft.x;

        int curPlotID = plotID1;          // The plot we are considering adding
        int topOfColumnID = plotID1;      // Keep track of the top of each column also
        Point curTopRight = getPlotTopRightFromID(curPlotID, chunkSize, plotsPerSide);
        Point curBottomLeft = getPlotBottomLeftFromID(curPlotID, chunkSize, plotsPerSide);
        while(curTopRight.x <= endTopRight.x) // From left to right, go down the columns
        {
            // Go down while the top of the plot is high enough to possibly be in the line
            while(pointStrictlyAboveLine(curTopRight, lowerSlope, lowerZint, 0.1))
            {
                // If we are at the destination, add it to the list and stop looking
                if(curPlotID == plotID2)
                {
                    ids.push_back(curPlotID);
                    break;
                }
                // If the bottom of the plot is low enough, then it is on the line
                if(!pointWeaklyAboveLine(curBottomLeft, higherSlope, higherZint))
                {
                    ids.push_back(curPlotID);
                }
                // Now look at the next plot down vertically
                curPlotID = idBelow(curPlotID, plotsPerSide);
                curTopRight = getPlotTopRightFromID(curPlotID, chunkSize, plotsPerSide);
                curBottomLeft = getPlotBottomLeftFromID(curPlotID, chunkSize, plotsPerSide);
            }
            // Move to the next column of plots
            topOfColumnID = idRight(topOfColumnID, plotsPerSide);
            curPlotID = topOfColumnID;
            curTopRight = getPlotTopRightFromID(curPlotID, chunkSize, plotsPerSide);
            curBottomLeft = getPlotBottomLeftFromID(curPlotID, chunkSize, plotsPerSide);
        }
    }
    return ids;
}

bool PlotNetwork::hasLineOfSight(int plotID1, int plotID2) const
{
    std::vector<int> idsBetween = getPlotIDsBetween(plotID1, plotID2);
    for(int id : idsBetween)
    {
        if(id2node.count(id) == 0)
        {
            return false;
        }
    }
    return true;
}

std::vector<int> PlotNetwork::clipPath(std::vector<int> path) const
{
    if(path.size() < 3)
    {
        return path;
    }
    std::vector<int> clippedPath;
    clippedPath.push_back(path[0]);

    int startIndex = 0;

    int lookingAtIndex = path.size() - 1; // Start by checking if we can go straight to the end
    while(lookingAtIndex > startIndex + 1)
    {
        // If we can go straight to where we are looking, add that
        // to the vector, and now start looking from the place we
        // just added
        if(hasLineOfSight(path[startIndex], path[lookingAtIndex]))
        {
            clippedPath.push_back(path[lookingAtIndex]);
            if(lookingAtIndex == path.size() - 1) // If we got to the very end, return now
            {
                return clippedPath;
            }
            startIndex = lookingAtIndex;
            lookingAtIndex = path.size() - 1;
        }
        else // If we can't see straight there, look a little closer
        {
            lookingAtIndex--;
            // If we are one step away from where we are looking,
            // then no clipping can be done from start index.
            // Advance to the next plot and start looking from there
            if(lookingAtIndex == startIndex + 1)
            {
                clippedPath.push_back(path[lookingAtIndex]);
                startIndex = lookingAtIndex;
                lookingAtIndex = path.size() - 1;
            }
        }

    }
    clippedPath.push_back(path.back()); // If we didn't get to the end, add the end in
    return clippedPath;
}
