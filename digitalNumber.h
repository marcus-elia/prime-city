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

    // The digits we have
    std::vector<Digit> digits;
public:
    DigitalNumber();
    DigitalNumber(Point inputCenter, RGBAcolor inputColor, Point &inputOwnerCenter,
                  int inputNumber, int inputMaxWidth, int inputMaxHeight);

    // Create the vector of Digit objects.
    void initializeDigits();

    void draw() const;

    // Setters
    void setColor(RGBAcolor inputColor);

    void move(double delta_x, double delta_y, double delta_z);
    void rotate(double thetaX, double thetaY, double thetaZ);
};

#endif //PRIME_CITY_DIGITALNUMBER_H
