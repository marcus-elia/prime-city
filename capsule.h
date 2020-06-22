#ifndef PRIME_CITY_CAPSULE_H
#define PRIME_CITY_CAPSULE_H

#include "solid.h"

class Capsule : public Solid
{
private:
    // The smoothness
    int pointsPerRing;
    int numRings;
public:
    Capsule();
    Capsule(Point inputCenter, RGBAcolor inputColor,
    double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor);
    Capsule(Point inputCenter, RGBAcolor inputColor,
    double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
    int inputNumRings, int inputPointsPerRing);
    Capsule(Point inputCenter, RGBAcolor inputColor,
    double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
    Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
    Point inputOwnerCenter);

    void initializeCorners();

    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};
#endif //PRIME_CITY_CAPSULE_H
