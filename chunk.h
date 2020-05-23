#ifndef PRIME_CITY_CHUNK_H
#define PRIME_CITY_CHUNK_H

#include "graphics.h"
#include "structs.h"
#include <experimental/optional>
//#include "building.h"
#include <vector>
#include <stdlib.h>
#include <time.h>




class Chunk
{
private:
    Point2D bottomLeft; // The bottom left coordinate divided by sideLength
    int sideLength;
    Point2D center;   // The actual center

    // The number of the chunk based on its location
    int chunkID;

    // The value of the Perlin noise map for this chunk
    double perlinSeed;

    // How many plots are in a side of the chunk (so 8 would mean 64  plots per chunk)
    const static int plotsPerSide = 8;
    int plotSize;

    // True if the plot at i,j is empty
    bool plotEmpty[plotsPerSide][plotsPerSide];

    //std::vector<Building> buildings;
public:
    Chunk();
    Chunk(Point2D inputBottomLeft, int inputSideLength, double inputPerlinSeed);

    void initializeCenter();
    void initializePlotEmpty();
    void makeBuildings();

    // Getters
    Point2D getBottomLeft() const;
    int getSideLength() const;
    Point2D getCenter() const;
    int getPlotsPerSide() const;
    int getPlotSize() const;
    //std::vector<Building> getBuildings() const;

    void draw() const;

    // Wrapper function, returns chunks around this
    std::vector<Point2D> getChunksAround(int radius);

    // Divides by size and converts resulting ordered pair to int
    int chunkToInt() const;


    // Calls correctCollision() on every Building in this chunk. Returns nullopt
    // if the point is not with buffer of a building, and returns a corrected Point
    // if otherwise.
    std::experimental::optional<Point> correctCollision(Point p, int buffer);

};

// Returns the result of mapping a 2d point in Z x Z into the
// non-negative integers, by spiraling clockwise.
int pointToInt(Point2D p);

// Returns the ints corresponding to to all chunks that are within radius of this one,
// using the taxicab metric
std::vector<int> getChunksAroundPoint(Point2D p, int radius);
std::vector<Point2D> getChunksAroundPointByPoint(Point2D p, int radius);


#endif //PRIME_CITY_CHUNK_H
