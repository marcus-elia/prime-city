#ifndef PRIME_CITY_PLOTNODE_H
#define PRIME_CITY_PLOTNODE_H

// The PlotNode class represents empty plots within chunks to be used for
// navigation by the Enemy's and the Computer

#include "structs.h"
#include "mathHelper.h"
#include <experimental/optional>

class PlotNode
{
private:
    Point2D chunkCoords; // specify which chunk it is in
    Point2D plotCoords;  // specify the exact plot

    int id; // a unique int based on the chunkCoords and plotCoords

    int chunkSideLength;
    int plotsPerSide;

    Point center;

    // If there is a neighboring empty plot in each direction
    bool hasLeft, hasRight, hasUp, hasDown;

    // The actual node in each direction, or nullopt
    std::experimental::optional<PlotNode*> leftNeighbor, rightNeighbor, upNeighbor, downNeighbor;
public:
    PlotNode();

    PlotNode(Point2D inputChunkCoords, Point2D inputPlotCoords, int inputChunkSideLength, int inputPlotsPerSide);

    PlotNode(int inputPlotID, int inputChunkSideLength, int inputPlotsPerSide);

    // Make the Center point based on the chunk and plot coords
    void initializeCenter();

    // Setters
    void addLeftNeighbor(std::experimental::optional<PlotNode*> inputLeftNeighbor);
    void addRightNeighbor(std::experimental::optional<PlotNode*> inputRightNeighbor);
    void addUpNeighbor(std::experimental::optional<PlotNode*> inputUpNeighbor);
    void addDownNeighbor(std::experimental::optional<PlotNode*> inputDownNeighbor);

    // Getters
    std::experimental::optional<PlotNode*> getLeftNeighbor() const;
    std::experimental::optional<PlotNode*> getRightNeighbor() const;
    std::experimental::optional<PlotNode*> getUpNeighbor() const;
    std::experimental::optional<PlotNode*> getDownNeighbor() const;

    bool hasLeftNeighbor() const;
    bool hasRightNeighbor() const;
    bool hasUpNeighbor() const;
    bool hasDownNeighbor() const;

    Point2D getChunkCoords() const;
    Point2D getPlotCoords() const;
    Point getCenter() const;
    int getID() const;
};

#endif //PRIME_CITY_PLOTNODE_H
