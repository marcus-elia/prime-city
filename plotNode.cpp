#include "plotNode.h"

PlotNode::PlotNode()
{
    chunkCoords = {0, 0};
    plotCoords = {0, 0};
    initializeCenter();
    leftNeighbor = std::experimental::nullopt;
    rightNeighbor = std::experimental::nullopt;
    upNeighbor = std::experimental::nullopt;
    downNeighbor = std::experimental::nullopt;
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