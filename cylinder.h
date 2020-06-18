#ifndef PRIME_CITY_CYLINDER_H
#define PRIME_CITY_CYLINDER_H

#include "solid.h"
#include <iostream>

// Creates an elliptical cylinder whose bases are parallel to the xz plane
// NOTE: in order for collision to work, must be a circular ellipse

// xWidth and zWidth are the dimensions of the bottom ellipse, and
// topXWidth and topZWidth are for the top ellipse

class Cylinder : public Solid
{
private:
    const static int smoothness = 24;
    double topXWidth;
    double topZWidth;

    const static int distanceBetweenHighLines = 8;
    const static int distanceBetweenMediumLines = 32;
    const static int distanceBetweenLowLines = 56;

    std::vector<std::vector<Point>> linePoints;
public:
    Cylinder();
    Cylinder(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             linesDrawnEnum inputLinesDrawn=Normal);
    Cylinder(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             double inputTopXWidth, double inputTopZWidth,
             linesDrawnEnum inputLinesDrawn=Normal);
    Cylinder(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
             Point inputOwnerCenter,
             linesDrawnEnum inputLinesDrawn=Normal);
    Cylinder(Point inputCenter, RGBAcolor inputColor,
             double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
             double inputTopXWidth, double inputTopZWidth,
             Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
             Point inputOwnerCenter,
             linesDrawnEnum inputLinesDrawn=Normal);

    // Make the corners, which in this case are the points along the circumference of the
    // top and bottom ellipses (the 2i th entry is a top point, and the 2i+1 th entry is the
    // bottom point corresponding to it).
    void initializeCorners();

    // Make points for drawing gridlines, if applicable
    void initializeLinePoints();

    // Getters
    double getTopXWidth() const;
    double getTopZWidth() const;

    // Setters
    void setTopXWidth(double inputTopXWidth);
    void setTopZWidth(double inputTopZWidth);

    // Geometry
    // Returns the x or z radius at a height of y above the base
    double getXRadiusAtHeight(double y) const;
    double getZRadiusAtHeight(double y) const;
    // Get the coordinates on circumference at given height and angle
    Point getPointAtHeight(double y, double theta) const;

    void lookAt(Point &p);

    void draw() const;
    void drawLines() const;
    void drawFaces() const;
    void drawGridLines() const;

    std::experimental::optional<Point> correctCollision(Point p, int buffer);

    void printDebugStats();
};

// Returns the focus distance sum of p away from the ellipse centered at c
// with xw and zw as the diameters. Working in the xz-plane.
double focusDistanceSum(Point p, Point c, double xw, double zw);

// If the Point p is within buffer units of the cylinder thing defined by center c, and
// bottom diameters xw zw and top diameters topxw topzw and height yw, then return the corrected
// version of the point.  If p is not too close, then return nullopt.
std::experimental::optional<Point> correctCylinderCollision(Point p, int buffer, Point c,
                                                                    double xw, double yw, double zw,
                                                                    double topxw, double topzw);

// For each Point here, consider only the x and z coordinates. Assuming p lies in the same plane
// as the ellipse determined by c, xw, and zw, this returns the corrected version of where p
// should be if p is too close to the ellipse. Likewise, ignore the y-coordinate of the returned
// Point.
std::experimental::optional<Point> correctEllipticalCrossSection(Point p, int buffer, Point c,
                                                                  double xw, double zw);


#endif //PRIME_CITY_CYLINDER_H
