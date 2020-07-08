#include "explosion.h"

Explosion::Explosion()
{
    location = {0, 10, 0};
    currentRadius = 1;
    lifeTime = 60;
    currentColor = {1,0,0,1};
    deltaRadius = 2;
    number = std::experimental::nullopt;
    deltaAlpha = currentColor.a / lifeTime;
    sphere = Ellipsoid(location, currentColor, 2*currentRadius, 2*currentRadius, 2*currentRadius, currentColor);
}
// If inputNumber is negative, then number = nullopt
Explosion::Explosion(Point inputLocation, double inputRadius, int inputLifeTime, RGBAcolor inputColor, double inputDeltaRadius,
int inputNumber)
{
    location = inputLocation;
    currentRadius = inputRadius;
    lifeTime = inputLifeTime;
    age = 0;
    currentColor = inputColor;
    deltaRadius = inputDeltaRadius;
    if(inputNumber > 0)
    {
        number = inputNumber;
    }
    else
    {
        number = std::experimental::nullopt;
    }
    deltaAlpha = currentColor.a / lifeTime;
    sphere = Ellipsoid(location, currentColor, 2*currentRadius, 2*currentRadius, 2*currentRadius, currentColor);
}

void Explosion::tick()
{
    currentRadius += deltaRadius;
    sphere.setRadii(2*currentRadius, 2*currentRadius, 2*currentRadius);
    currentColor.a -= deltaAlpha;
    sphere.setColor(currentColor);
    age++;
}
void Explosion::draw() const
{
    sphere.drawFaces();
}

std::experimental::optional<int> Explosion::getNumber() const
{
    return number;
}
bool Explosion::isDone() const
{
    return age == lifeTime;
}
// Returns true if the point is in the sphere
bool Explosion::containsPoint(Point p) const
{
    return false;
}