#include "enemy.h"

Enemy::Enemy()
{
    location = {0, 6, 0};
    bodyHeight = 12;
    radius = 4;
    speed = 2;
    rotationSpeed = 0.1;
    xzAngle = 3*PI/2;
    yAngle = 0;

    velocity = {0,0,0}; // current x y and z velocity

    number = 123;

    initializeSolids();
}
Enemy::Enemy(Point inputLocation, double inputBodyHeight, double inputRadius,
             double inputSpeed, double inputRotationSpeed, int inputNumber)
{
    location = inputLocation;
    bodyHeight = inputBodyHeight;
    radius = inputRadius;
    speed = inputSpeed;
    rotationSpeed = inputRotationSpeed;
    number = inputNumber;
    initializeSolids();
}

void Enemy::initializeSolids()
{
    RGBAcolor edgeColor = {1, 1, 1, 1};
    Point center = {location.x, location.y, location.z};
    solids.push_back(std::make_shared<Capsule>(Capsule(center, bodyColor,
                                                         2*radius, bodyHeight, 2*radius, edgeColor)));
    center.y += bodyHeight/2 + radius;
    solids.push_back(std::make_shared<Ellipsoid>(Ellipsoid(center, headColor,
                                                         2*radius, 2*radius, 2*radius, edgeColor)));
}

Point Enemy::getLocation() const
{
    return location;
}

void Enemy::setFutureLocations(std::vector<Point> inputFutureLocations)
{
    futureLocations = inputFutureLocations;
    arriveAtTarget();
}

void Enemy::turnTowardTarget()
{
    xzAngle = atan2(targetLocation.z - location.z, targetLocation.x - location.x);
    velocity = {speed*cos(xzAngle), 0, speed*sin(xzAngle)};
}


void Enemy::arriveAtTarget()
{
    if(!futureLocations.empty())
    {
        targetLocation = futureLocations.back();
        futureLocations.pop_back();
        turnTowardTarget();
    }
}

void Enemy::tick()
{
    if(distance2d(location, targetLocation) < speed)
    {
        location = targetLocation;
        arriveAtTarget();
    }
    else
    {
        location.x += velocity.x;
        location.z += velocity.z;
    }
}


void Enemy::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
}