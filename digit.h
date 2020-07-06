#ifndef PRIME_CITY_DIGIT_H
#define PRIME_CITY_DIGIT_H

#include "structs.h"
#include "graphics.h"
#include "movable.h" // for rotatePointAroundPoint
#include <vector>
#include <memory>

/*
 * Each digit is primarily a numerical digit (0-9)
 * with functionality to render it using a
 * seven-segment display.
 *       1
 *       _
 *    6 |_| 2
 *    5 |_| 3
 *       4
 *  (7 is the middle segment)
 */

class Digit
{
private:
    int digit;

    Point center;
    Point ownerCenter;
    RGBAcolor color;

    double xzAngle;

    // The width and height that the digit will take up
    double width;
    double height;
    // The thickness of the segments
    double thickness;

    // Each group of 6 points determines a segment
    // (so the length of corners is a multiple of 6)
    std::vector<Point> corners;

    // Record which segments make an int
    static std::vector<std::vector<int>> segmentsVector;
public:
    Digit();
    Digit(Point inputCenter, RGBAcolor inputColor, Point &inputOwnerCenter, int inputDigit,
          double inputWidth, double inputHeight, double inputThickness);

    void initializeCorners();

    void setColor(RGBAcolor inputColor);
    void setXZAngle(double inputAngle); // Rotates around y-axis so xzAngle = the input

    void draw() const;
    void move(double deltaX, double deltaY, double deltaZ);
    void moveSelfAndOwner(double deltaX, double deltaY, double deltaZ); // also update ownerCenter
    void rotate(double thetaX, double thetaY, double thetaZ);
    void rotateAroundOwner(double thetaX, double thetaY, double thetaZ);
};

#endif //PRIME_CITY_DIGIT_H
