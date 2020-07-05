#ifndef PRIME_CITY_BUILDING_H
#define PRIME_CITY_BUILDING_H

#include "solid.h"
#include "recPrism.h"
#include "cylinder.h"
#include "frustum.h"
#include <vector>
#include <memory>

class Building
{
protected:
    std::vector<std::unique_ptr<Solid>> solids;

    // The rectangular base of the building's property
    Point2D topLeft;
    int sideLength;

    int height;
    RGBAcolor color;
    RGBAcolor edgeColor;

    const static bool GRIDLINES_ENABLED = false;

public:
    Building();

    Building(Point2D inputTopLeft, int inputSideLength, int inputHeight,
             RGBAcolor inputColor, RGBAcolor inputEdgeColor);

    void initializeSolids();
    // Create the solids, using a different helper function depending on how many solids there are
    void initializeSolids1();
    void initializeSolids2();
    void initializeSolids3();
    void initializeSolids4();

    // This adds a pointer to a new solid to the vector of solids. The new solid has its center y coordinate at
    // center height, and its height is ywidth. The x and z dimensions are also specified.
    // The seeds must be between 0 and 100 so that rectPrismSeed < cylinderSeed. A random int between
    // 0 and 100 will be created so that if r < rectPrismSeed, the solid is a rectprism. Else if r < cylinderSeed,
    // it's a cylinder. Otherwise, it's a frustum.
    void addRandomSolid(int centerHeight, int yWidth, int xWidth, int zWidth, int topXWidth, int topZWidth,
            int rectPrismSeed, int cylinderSeed, linesDrawnEnum lineDensity);

    // Get a random line density to help initialize solids
    linesDrawnEnum getRandomLineDensity() const;

    // Getters
    std::vector<std::unique_ptr<Solid>> getSolids() const;

    void draw() const;

    // Check each solid for a collision with this point (or if the point is within
    // buffer of the solid). If it finds one, it returns that solid's corrected point,
    // and doesn't check any more. If none of the solids have a problem, it returns nullopt.
    std::experimental::optional<Point> correctCollision(Point p, int buffer);
};

#endif //PRIME_CITY_BUILDING_H
