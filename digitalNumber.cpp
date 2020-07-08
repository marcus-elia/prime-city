#include "digitalNumber.h"

DigitalNumber::DigitalNumber()
{
    number = 0;
    initializeDigits();
}
DigitalNumber::DigitalNumber(Point inputCenter, RGBAcolor inputColor, Point &inputOwnerCenter,
                             int inputNumber, int inputMaxWidth, int inputMaxHeight)
{
    if(number >= 0)
    {
        number = inputNumber;
    }
    else
    {
        number = -inputNumber;
    }
    numberIsPrime = twoDigitIsPrime(number);
    center = inputCenter;
    ownerCenter = inputOwnerCenter;
    xzAngle = PI/2;
    color = inputColor;
    maxWidth = inputMaxWidth;
    maxHeight = inputMaxHeight;
    initializeDigits();
}

void DigitalNumber::initializeDigits()
{
    // First, get a list of digits. So 341 becomes {1,4,3}
    std::vector<int> decimalDigits;
    decimalDigits.push_back(number % 10);
    int x = number - number % 10;
    x = x / 10;
    while(x != 0)
    {
        decimalDigits.push_back(x % 10);
        x = x - x % 10;
        x = x / 10;
    }

    // Now create Digit objects in the correct locations
    int d = decimalDigits.size();
    double digitWidth = 0.8 * maxWidth / (1.2*d - 0.2); // How wide each one is
    double digitHeight = 2*digitWidth;                  // How tall they are
    if(digitHeight > 0.8*maxHeight)
    {
        digitHeight = 0.8*maxHeight;
        digitWidth = digitHeight/2;
    }
    double gapSize = 0.2 * digitWidth;                  // How far apart they are

    double curCenterX;   // Keep track of where we are putting Digits
    if(d % 2 == 0)
    {
        curCenterX = center.x + gapSize/2.0 + (d/2.0-1)*digitWidth + (d/2.0-1)*gapSize + digitWidth/2.0;
    }
    else
    {
        curCenterX = center.x + (d-1)/2*gapSize + (d-1)/2*digitWidth;
    }

    // Create the digit objects in the correct places
    for(int i : decimalDigits)
    {
        digits.push_back(Digit({curCenterX, center.y, center.z}, color, center, i, digitWidth, digitHeight, digitWidth/5));

        // Move the location to the left
        curCenterX -= gapSize;
        curCenterX -= digitWidth;
    }
}

Point DigitalNumber::getCenter() const
{
    return center;
}
bool DigitalNumber::getNumberIsPrime() const
{
    return numberIsPrime;
}

void DigitalNumber::setColor(RGBAcolor inputColor)
{
    color = inputColor;
    for(Digit &d : digits)
    {
        d.setColor(inputColor);
    }
}
void DigitalNumber::setXZAngle(double inputAngle)
{
    rotate(0, inputAngle - xzAngle, 0);
    xzAngle = inputAngle;
}

void DigitalNumber::draw() const
{
    glDisable(GL_CULL_FACE);

    // Draw each digit
    for(Digit d : digits)
    {
        d.draw();
    }
}

void DigitalNumber::move(double deltaX, double deltaY, double deltaZ)
{
    // Move the center of the number
    center.x += deltaX;
    center.y += deltaY;
    center.z += deltaZ;

    // Move each individual digit
    for(Digit &d : digits)
    {
        d.moveSelfAndOwner(deltaX, deltaY, deltaZ);
    }
}

void DigitalNumber::moveSelfAndOwner(double deltaX, double deltaY, double deltaZ)
{
    move(deltaX, deltaY, deltaZ);
    ownerCenter.x += deltaX;
    ownerCenter.y += deltaY;
    ownerCenter.z += deltaZ;
}


void DigitalNumber::rotate(double thetaX, double thetaY, double thetaZ)
{
    for(Digit &d : digits)
    {
        d.rotateAroundOwner(thetaX, thetaY, thetaZ);
    }
}