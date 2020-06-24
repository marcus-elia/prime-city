#include "plotNode.h"

PlotNode::PlotNode()
{
    chunkCoords = {0, 0};
    plotCoords = {0, 0};
    initializeCenter();
}

PlotNode::PlotNode(Point2D inputChunkCoords, Point2D inputPlotCoords, int inputChunkSideLength, int inputPlotsPerSide)
{
    chunkCoords = inputChunkCoords;
    plotCoords = inputPlotCoords;
    chunkSideLength = inputChunkSideLength;
    plotsPerSide = inputPlotsPerSide;
    initializeCenter();
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
void PlotNode::addLeftNeighbor(std::experimental::optional<PlotNode*> inputLeftNeighbor);
void PlotNode::addRightNeighbor(std::experimental::optional<PlotNode*> inputRightNeighbor);
void PlotNode::addTopNeighbor(std::experimental::optional<PlotNode*> inputTopNeighbor);
void PlotNode::addBottomNeighbor(std::experimental::optional<PlotNode*> inputBottomNeighbor);

// Getters
std::experimental::optional<PlotNode*> PlotNode::getLeftNeighbor() const;
std::experimental::optional<PlotNode*> PlotNode::getRightNeighbor() const;
std::experimental::optional<PlotNode*> PlotNode::getTopNeighbor() const;
std::experimental::optional<PlotNode*> PlotNode::getBottomNeighbor() const;

bool PlotNode::hasLeftNeighbor() const;
bool PlotNode::hasRightNeighbor() const;
bool PlotNode::hasTopNeighbor() const;
bool PlotNode::hasBottomNeighbor() const;

Point2D PlotNode::getChunkCoords() const;
Point2D PlotNode::getPlotCoords() const;
Point PlotNode::getCenter() const;