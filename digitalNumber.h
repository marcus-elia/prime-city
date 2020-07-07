#ifndef PRIME_CITY_DIGITALNUMBER_H
#define PRIME_CITY_DIGITALNUMBER_H

#include "digit.h"

class DigitalNumber
{
private:
    // The number we are representing
    int number;

    Point center;
    Point ownerCenter;
    RGBAcolor color;

    // The dimensions we have to draw the number in
    int maxWidth;
    int maxHeight;

    double xzAngle;

    // The digits we have
    std::vector<Digit> digits;
public:
    DigitalNumber();
    DigitalNumber(Point inputCenter, RGBAcolor inputColor, Point &inputOwnerCenter,
                  int inputNumber, int inputMaxWidth, int inputMaxHeight);

    // Create the vector of Digit objects.
    void initializeDigits();

    Point getCenter() const;

    void setXZAngle(double inputAngle);

    void draw() const;

    // Setters
    void setColor(RGBAcolor inputColor);

    void move(double deltaX, double deltaY, double deltaZ);
    void moveSelfAndOwner(double deltaX, double deltaY, double deltaZ);
    void rotate(double thetaX, double thetaY, double thetaZ);
};

#endif //PRIME_CITY_DIGITALNUMBER_H
