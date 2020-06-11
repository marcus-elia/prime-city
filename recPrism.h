#ifndef PRIME_CITY_RECPRISM_H
#define PRIME_CITY_RECPRISM_H

#include "solid.h"
#include <iostream>
#include <stdexcept>

class RecPrism : public Solid
{
private:
    const static int distanceBetweenHighLines = 8;
    const static int distanceBetweenMediumLines = 16;
    const static int distanceBetweenLowLines = 24;
    // Points for drawing extra gridlines on the faces of the rectangular prism
    std::vector<Point> xLinePoints;
    std::vector<Point> yLinePoints;
    std::vector<Point> zLinePoints;
public:
    RecPrism();
    RecPrism(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             linesDrawnEnum inputLinesDrawn=Normal);
    RecPrism(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
             Point inputOwnerCenter, linesDrawnEnum inputLinesDrawn=Normal);

    // Make the corners of the rec prism
    void initializeCorners();

    // Make points for drawing gridlines, if applicable
    void initializeLinePoints();
    void initializeXLinePoints();
    void initializeYLinePoints();
    void initializeZLinePoints();

    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;
    void drawGridLines() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};

std::experimental::optional<Point> correctRectangularPrismCollision(Point p, int buffer, Point c,
                                                                    double xw, double yw, double zw);

#endif //PRIME_CITY_RECPRISM_H
