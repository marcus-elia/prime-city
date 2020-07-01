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

PlotNode::PlotNode(int inputPlotID, int inputChunkSideLength, int inputPlotsPerSide)
{
    plotsPerSide = inputPlotsPerSide;
    id = inputPlotID;
    chunkCoords = chunkIDtoPoint(id / (plotsPerSide*plotsPerSide));
    plotCoords = idToPlotCoords(id, plotsPerSide);
    chunkSideLength = inputChunkSideLength;
    initializeCenter();
    leftNeighbor = std::experimental::nullopt;
    rightNeighbor = std::experimental::nullopt;
    upNeighbor = std::experimental::nullopt;
    downNeighbor = std::experimental::nullopt;
}

void PlotNode::initializeCenter()
{
    int topLeftX = chunkCoords.x * chunkSideLength;
    int topLeftZ = chunkCoords.z * chunkSideLength;
    int plotSideLength = chunkSideLength / plotsPerSide;
    double centerX = topLeftX + plotCoords.x*plotSideLength + plotSideLength/2;
    double centerZ = topLeftZ + plotCoords.z*plotSideLength + plotSideLength/2;
    center = {centerX, 0, centerZ};
}

// Setters
void PlotNode::addLeftNeighbor(PlotNode &inputLeftNeighbor)
{
    leftNeighbor = &inputLeftNeighbor;
}
void PlotNode::addRightNeighbor(PlotNode &inputRightNeighbor)
{
    rightNeighbor = &inputRightNeighbor;
}
void PlotNode::addUpNeighbor(PlotNode &inputUpNeighbor)
{
    upNeighbor = &inputUpNeighbor;
}
void PlotNode::addDownNeighbor(PlotNode &inputDownNeighbor)
{
    downNeighbor = &inputDownNeighbor;
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
    return leftNeighbor != std::experimental::nullopt;
}
bool PlotNode::hasRightNeighbor() const
{
    return rightNeighbor != std::experimental::nullopt;
}
bool PlotNode::hasUpNeighbor() const
{
    return upNeighbor != std::experimental::nullopt;
}
bool PlotNode::hasDownNeighbor() const
{
    return downNeighbor != std::experimental::nullopt;
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
