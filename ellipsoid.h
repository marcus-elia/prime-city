#ifndef PRIME_CITY_ELLIPSOID_H
#define PRIME_CITY_ELLIPSOID_H

#include "solid.h"

class Ellipsoid : public Solid
{
private:
    // The smoothness
    int pointsPerRing;
    int numRings;
    bool hideLines;
public:
    Ellipsoid();
    Ellipsoid(Point inputCenter, RGBAcolor inputColor,
              double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor);
    Ellipsoid(Point inputCenter, RGBAcolor inputColor,
              double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
              bool inputHideLines, int inputNumRings, int inputPointsPerRing);
    Ellipsoid(Point inputCenter, RGBAcolor inputColor,
              double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
              Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
              Point inputOwnerCenter);

    void initializeCorners();

    void lookAt(Point &p);

    // This causes corners to be re-created
    void setRadii(double inputXWidth, double inputYWidth, double inputZWidth);

    void setColor(RGBAcolor inputColor);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};

#endif //PRIME_CITY_ELLIPSOID_H
