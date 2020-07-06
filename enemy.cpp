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
    Point headCenter = {location.x, location.y + bodyHeight/2 + radius, location.z};
    dn = DigitalNumber(headCenter, {1,1,1,1}, headCenter, number, radius, radius);
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
    velocity = {0,0,0};
    initializeSolids();
    Point headCenter = {location.x, location.y + bodyHeight/2 + radius, location.z};
    dn = DigitalNumber(headCenter, {1,1,1,1}, headCenter, number, 1.5*radius, radius);
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

void Enemy::move()
{
    for(auto s : solids)
    {
        s->move(velocity.x, velocity.y, velocity.z);
    }
    location.x += velocity.x;
    location.y += velocity.y;
    location.z += velocity.z;
    dn.moveSelfAndOwner(velocity.x, velocity.y, velocity.z);
    if(location.x != dn.getCenter().x || location.z != dn.getCenter().z)
    {
        int a = 1;
    }
}


void Enemy::arriveAtTarget()
{
    if(!futureLocations.empty())
    {
        targetLocation = futureLocations.back();
        futureLocations.pop_back();
        // If the current location is closer to the ultimate goal than the next step, skip it
        if(!futureLocations.empty() && distance2d(location, futureLocations[0]) < distance2d(targetLocation, futureLocations[0]))
        {
            targetLocation = futureLocations.back();
            futureLocations.pop_back();
        }

        turnTowardTarget();
    }
}

void Enemy::tick()
{
    if(distance2d(location, targetLocation) < speed)
    {
        for(auto s : solids)
        {
            s->move(targetLocation.x - location.x,
                    0,
                    targetLocation.z - location.z);
        }
        dn.moveSelfAndOwner(targetLocation.x - location.x,
                0,
                targetLocation.z - location.z);
        location = targetLocation;
        arriveAtTarget();
    }
    else
    {
        move();
    }
    if(location.x != dn.getCenter().x || location.z != dn.getCenter().z)
    {
        int a = 1;
    }
}


void Enemy::draw() const
{
    dn.draw();
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
}

bool Enemy::isHitByMissile(Point missileLoc, double missileRadius)
{
    bool lowEnough =  missileLoc.y < bodyHeight + 2*radius - missileRadius/2;
    bool closeEnough = distance2d(location, missileLoc) < missileRadius/2 + radius;
    return  closeEnough && lowEnough;
}