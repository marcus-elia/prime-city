#include "missile.h"

Missile::Missile()
{
    location = {0, 100, 0};
    speed = 1;
    velocity = {0, -1, 0};
    rotationalVelocity = {1,1,1};
    radius = 10;
    wasShotByPlayer = true;
    cube = RecPrism();
}
Missile::Missile(Point inputLocation, double inputSpeed, Point inputVelocity, double inputRadius, bool inputWasShotByPlayer,
        RGBAcolor inputCoreColor)
{
    location = inputLocation;
    speed = inputSpeed;
    setVelocity(inputVelocity);
    rotationalVelocity = {1,1,1};
    radius = inputRadius;
    wasShotByPlayer = inputWasShotByPlayer;
    coreColor = inputCoreColor;
    cube = RecPrism(location, {1,1,1,1}, radius, radius, radius, {1,1,1,1});
    core = Ellipsoid(location, coreColor, radius, radius, radius, coreColor);
}

// Getters
Point Missile::getLocation() const
{
    return location;
}
double Missile::getSpeed() const
{
    return speed;
}
Point Missile::getVelocity() const
{
    return velocity;
}
double Missile::getRadius() const
{
    return radius;
}
RGBAcolor Missile::getCoreColor() const
{
    return coreColor;
}
bool Missile::getWasShotByPlayer() const
{
    return wasShotByPlayer;
}

// Setters
void Missile::setLocation(Point inputLocation)
{
    location = inputLocation;
}
void Missile::setSpeed(double inputSpeed)
{
    speed = inputSpeed;
}
void Missile::setVelocity(Point inputVelocity)
{
    double normalization = sqrt(inputVelocity.x*inputVelocity.x + inputVelocity.y*inputVelocity.y +
                                inputVelocity.z*inputVelocity.z);
    velocity.x = inputVelocity.x / normalization * speed;
    velocity.y = inputVelocity.y / normalization * speed;
    velocity.z = inputVelocity.z / normalization * speed;
}
void Missile::setRadius(double inputRadius)
{
    radius = inputRadius;
}

void Missile::draw() const
{
    cube.drawLines();
    core.drawFaces();
}

void Missile::tick()
{
    location.x += velocity.x;
    location.y += velocity.y;
    location.z += velocity.z;
    cube.move(velocity.x, velocity.y, velocity.z);
    cube.rotate(rotationalVelocity.x, rotationalVelocity.y, rotationalVelocity.z);
    core.move(velocity.x, velocity.y, velocity.z);
}
bool Missile::isOutOfBounds(Point playerLocation, int maxDistance)
{
    return location.y < 0 || location.y > 2048 || distance2d(location, playerLocation) > maxDistance;
}