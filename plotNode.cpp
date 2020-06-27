#include "plotNode.h"

PlotNode::PlotNode()
{
    chunkCoords = {0, 0};
    plotCoords = {0, 0};
    chunkSideLength = 512;
    plotsPerSide = 8;
    initializeCenter();
    leftNeighbor = std::experimental::nullopt;
    rightNeighbor = std::experimental::nullopt;
    upNeighbor = std::experimental::nullopt;
    downNeighbor = std::experimental::nullopt;
    id = makeID(chunkCoords, plotCoords, plotsPerSide);
}

PlotNode::PlotNode(Point2D inputChunkCoords, Point2D inputPlotCoords, int inputChunkSideLength, int inputPlotsPerSide)
{
    chunkCoords = inputChunkCoords;
    plotCoords = inputPlotCoords;
    chunkSideLength = inputChunkSideLength;
    plotsPerSide = inputPlotsPerSide;
    initializeCenter();
    leftNeighbor = std::experimental::nullopt;
    rightNeighbor = std::experimental::nullopt;
    upNeighbor = std::experimental::nullopt;
    downNeighbor = std::experimental::nullopt;
    id = makeID(chunkCoords, plotCoords, plotsPerSide);
}

void PlotNode::initializeCenter()
{
    int topLeftX = chunkCoords.x * chunkSideLength;
    int topLeftZ = (chunkCoords.z + 1) * chunkSideLength;
    int plotSideLength = chunkSideLength / plotsPerSide;
    double centerX = topLeftX + plotCoords.x*plotSideLength + plotSideLength/2;
    double centerZ = topLeftZ + plotCoords.z*plotSideLength + plotSideLength/2;
    center = {centerX, 0, centerZ};
}

// Setters
void PlotNode::addLeftNeighbor(std::experimental::optional<PlotNode*> inputLeftNeighbor)
{
    leftNeighbor = inputLeftNeighbor;
}
void PlotNode::addRightNeighbor(std::experimental::optional<PlotNode*> inputRightNeighbor)
{
    rightNeighbor = inputRightNeighbor;
}
void PlotNode::addUpNeighbor(std::experimental::optional<PlotNode*> inputUpNeighbor)
{
    upNeighbor = inputUpNeighbor;
}
void PlotNode::addDownNeighbor(std::experimental::optional<PlotNode*> inputDownNeighbor)
{
    downNeighbor = inputDownNeighbor;
}

// Getters
std::experimental::optional<PlotNode*> PlotNode::getLeftNeighbor() const
{
    return leftNeighbor;
}
std::experimental::optional<PlotNode*> PlotNode::getRightNeighbor() const
{
    return rightNeighbor;
}
std::experimental::optional<PlotNode*> PlotNode::getUpNeighbor() const
{
    return upNeighbor;
}
std::experimental::optional<PlotNode*> PlotNode::getDownNeighbor() const
{
    return downNeighbor;
}

bool PlotNode::hasLeftNeighbor() const
{
    return leftNeighbor == std::experimental::nullopt;
}
bool PlotNode::hasRightNeighbor() const
{
    return rightNeighbor == std::experimental::nullopt;
}
bool PlotNode::hasUpNeighbor() const
{
    return upNeighbor == std::experimental::nullopt;
}
bool PlotNode::hasDownNeighbor() const
{
    return downNeighbor == std::experimental::nullopt;
}

Point2D PlotNode::getChunkCoords() const
{
    return chunkCoords;
}
Point2D PlotNode::getPlotCoords() const
{
    return plotCoords;
}
Point PlotNode::getCenter() const
{
    return center;
}
int PlotNode::getID() const
{
    return id;
}

int makeID(Point2D chunkCoords, Point2D plotCoords, int plotsPerSide)
{
    return chunkCoords.toChunkID() + plotCoords.x + plotCoords.z * plotsPerSide;
}
int makeID(int chunkID, Point2D plotCoords, int plotsPerSide)
{
    return chunkID + plotCoords.x + plotCoords.z * plotsPerSide;
}

Point2D idToPlotCoords(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    id = id % plotsPerChunk;
    int x = id % plotsPerSide;
    int z = id / plotsPerSide;
    return {x, z};
}

int idAbove(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    Point2D plotCoords = idToPlotCoords(id, plotsPerSide);
    int chunkID = id / plotsPerChunk;
    int x = plotCoords.x;
    int z = plotCoords.z;
    if(z > 0) // If it's in the same chunk
    {
        return makeID(chunkID, {x, z-1}, plotsPerSide);
    }
    else     // If it's in the chunk above this one
    {
        Point2D chunkCoords = chunkIDtoPoint(chunkID);
        chunkCoords.z -= 1; // move up one chunk
        return makeID(chunkCoords, {x, plotsPerSide-1}, plotsPerSide);
    }
}
int idBelow(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    Point2D plotCoords = idToPlotCoords(id, plotsPerSide);
    int chunkID = id / plotsPerChunk;
    int x = plotCoords.x;
    int z = plotCoords.z;
    if(z < plotsPerSide - 1) // If it's in the same chunk
    {
        return makeID(chunkID, {x, z+1}, plotsPerSide);
    }
    else     // If it's in the chunk above this one
    {
        Point2D chunkCoords = chunkIDtoPoint(chunkID);
        chunkCoords.z += 1; // move down one chunk
        return makeID(chunkCoords, {x, 0}, plotsPerSide);
    }
}
int idLeft(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    Point2D plotCoords = idToPlotCoords(id, plotsPerSide);
    int chunkID = id / plotsPerChunk;
    int x = plotCoords.x;
    int z = plotCoords.z;
    if(x > 0) // If it's in the same chunk
    {
        return makeID(chunkID, {x-1, z}, plotsPerSide);
    }
    else     // If it's in the chunk above this one
    {
        Point2D chunkCoords = chunkIDtoPoint(chunkID);
        chunkCoords.x -= 1; // move left one chunk
        return makeID(chunkCoords, {plotsPerSide-1, z}, plotsPerSide);
    }
}
int idRight(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    Point2D plotCoords = idToPlotCoords(id, plotsPerSide);
    int chunkID = id / plotsPerChunk;
    int x = plotCoords.x;
    int z = plotCoords.z;
    if(x < plotsPerSide-1) // If it's in the same chunk
    {
        return makeID(chunkID, {x+1, z}, plotsPerSide);
    }
    else     // If it's in the chunk above this one
    {
        Point2D chunkCoords = chunkIDtoPoint(chunkID);
        chunkCoords.x += 1; // move right one chunk
        return makeID(chunkCoords, {0, z}, plotsPerSide);
    }
}