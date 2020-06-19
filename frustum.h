#ifndef PRIME_CITY_FRUSTUM_H
#define PRIME_CITY_FRUSTUM_H

#include "structs.h"
#include "recPrism.h"
#include "solid.h"

class Frustum : public Solid
{
private:
    double upperXWidth;
    double upperZWidth;

    const static int distanceBetweenHighLines = 8;
    const static int distanceBetweenMediumLines = 16;
    const static int distanceBetweenLowLines = 24;
    // Points for drawing extra gridlines on the faces
    std::vector<Point> xLinePoints;
    std::vector<Point> yLinePoints;
    std::vector<Point> zLinePoints;
public:
    Frustum();

    Frustum(Point inputCenter, RGBAcolor inputColor,
            double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
            double inputUpperXWidth, double inputUpperZWidth,
            linesDrawnEnum inputLinesDrawn=Normal);

    Frustum(Point inputCenter, RGBAcolor inputColor,
            double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
            double inputUpperXWidth, double inputUpperZWidth,
            Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
            Point inputOwnerCenter,
            linesDrawnEnum inputLinesDrawn=Normal);

    void initializeCorners();
    // Make points for drawing gridlines, if applicable
    void initializeLinePoints();
    void initializeXLinePoints();
    void initializeYLinePoints();
    void initializeZLinePoints();

    // Geometry
    // Returns the x or z width at a height of y above the base
    double getXWidthAtHeight(double y) const;
    double getZWidthAtHeight(double y) const;

    void lookAt(Point &p);

    void draw() const;

    void drawLines() const;

    void drawFaces() const;

    void drawGridLines() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};

// If the Point p is within buffer units of the frustum defined by center c, and bottom
// side widths xw, zw, height yw, and top side widths topxw, topzw,
// return the corrected version of where p should be. If p is not
// too close, then return nullopt.
std::experimental::optional<Point> correctFrustumCollision(Point p, int buffer, Point c,
        double xw, double yw, double zw, double topxw, double topzw);

#endif //PRIME_CITY_FRUSTUM_H
