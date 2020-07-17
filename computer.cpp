#include "computer.h"

Computer::Computer()
{
    location = {0, 12, 0};
    speed = 2;
    velocity = {0, 0, 0};
    xzAngle = PI/2;
    targetAngle = 0;
    rotationSpeed = 0.02;

    radius = 8;
    bodyHeight = 24;

    bodyColor = {1, 0.8, 0, 1};
    headColor = {0.42, 0.2, 0.48, 1};
    initializeSolids();

    targetLocation = location;
    playerLocation = {0, 0, 0};

    missileTarget = {0, 0, 0};
    canShootTarget = false;
    needsToRotate = true;

    // Point to GameManager's vector of enemies
    std::vector<std::shared_ptr<Enemy>>* enemies = nullptr;
}
Computer::Computer(Point inputLocation, double inputSpeed, double inputRotationSpeed, std::vector<std::shared_ptr<Enemy>>* inputEnemies)
{
    location = inputLocation;
    speed = inputSpeed;
    velocity = {0, 0, 0};
    rotationSpeed = inputRotationSpeed;
    enemies = inputEnemies;

    xzAngle = PI/2;
    targetAngle = PI/2;

    bodyColor = {1, 0.8, 0, 1};
    headColor = {0.42, 0.2, 0.48, 1};
    initializeSolids();

    targetLocation = location;
    playerLocation = {0, 0, 0};

    missileTarget = {0, 0, 0};
    canShootTarget = true;
    needsToRotate = true;
}

void Computer::initializeSolids()
{
    RGBAcolor edgeColor = {1, 1, 1, 1};
    Point center = {location.x, location.y, location.z};
    solids.push_back(std::make_shared<Capsule>(Capsule(center, bodyColor,
                                                       2*radius, bodyHeight, 2*radius, edgeColor)));
    center.y += bodyHeight/2 + radius;
    solids.push_back(std::make_shared<Ellipsoid>(Ellipsoid(center, headColor,
                                                           2*radius, 2*radius, 2*radius, edgeColor)));

    // Left Arm
    /*double armRadius = 4;
    double armLength = 20;
    center = {location.x + radius + armRadius, 2*bodyHeight/3, location.z + armLength/2};
    std::shared_ptr<Capsule> leftArm = std::make_shared<Capsule>(Capsule(center, headColor,
                                                                         2*armRadius, armLength, 2*armRadius, edgeColor));
    leftArm->setOwnerCenter(location);
    leftArm->rotate(PI/2, 0, 0);
    solids.push_back(leftArm);

    center = {location.x, location.y, location.z + radius + 8};
    solids.push_back(std::make_shared<Ellipsoid>(Ellipsoid(center, {1,0,0,1},
                                                           15, 15, 15, edgeColor)));
    solids.back()->setOwnerCenter(location);*/
}

// Getters
Point Computer::getLocation() const
{
    return location;
}
double Computer::getRadius() const
{
    return radius;
}
RGBAcolor Computer::getHeadColor() const
{
    return headColor;
}
RGBAcolor Computer::getBodyColor() const
{
    return bodyColor;
}
bool Computer::getCanShootTarget() const
{
    return canShootTarget;
}
Point Computer::getMissileTarget() const
{
    return missileTarget;
}

bool Computer::isHitByMissile(Point missileLoc, double missileRadius) const
{
    bool lowEnough =  missileLoc.y < bodyHeight + 2*radius + missileRadius/2;
    bool closeEnough = distance2d(location, missileLoc) < missileRadius/2 + radius;
    return closeEnough && lowEnough;
}

// Setters
void Computer::setFutureLocations(std::vector<Point> inputFutureLocations)
{
    futureLocations = inputFutureLocations;
}
void Computer::setPlayerLocation(Point inputPlayerLocation)
{
    playerLocation = inputPlayerLocation;
    missileTarget = playerLocation;
    updateTargetAngle();
}

void Computer::tick()
{
    if(distance2d(location, targetLocation) < speed)
    {
        for(auto s : solids)
        {
            s->move(targetLocation.x - location.x,
                    0,
                    targetLocation.z - location.z);
        }
        location.x = targetLocation.x;
        location.z = targetLocation.z;
        arriveAtTarget();
    }
    else
    {
        //move();
    }


    // Rotate to where the computer is looking
    if(needsToRotate)
    {
        turn();
    }
    updateTargetAngle();
    updateCanShootTarget();
}

void Computer::turnVelocityTowardTarget()
{
    xzAngle = atan2(targetLocation.z - location.z, targetLocation.x - location.x);
    velocity = {speed*cos(xzAngle), 0, speed*sin(xzAngle)};
}

void Computer::move()
{
    for(auto s : solids)
    {
        s->moveSelfAndOwner(velocity.x, velocity.y, velocity.z);
    }
    location.x += velocity.x;
    location.y += velocity.y;
    location.z += velocity.z;
    needsToRotate = true;
}

void Computer::turn()
{
    double dif = angleSweptOut(xzAngle, targetAngle);
    if(dif < rotationSpeed)
    {
        rotate(targetAngle - xzAngle);
        needsToRotate = false;
    }
    else if(dif > 2*PI - rotationSpeed)
    {
        rotate(xzAngle - targetAngle);
        needsToRotate = false;
    }
    else if(dif < PI)
    {
        rotate(rotationSpeed);
    }
    else
    {
        rotate(-rotationSpeed);
    }
}

void Computer::rotate(double deltaXZAngle)
{
    // Make sure the resulting angle is between 0 and 2*PI
    xzAngle += deltaXZAngle;
    while(xzAngle >= 2*PI)
    {
        xzAngle -= 2*PI;
    }
    while(xzAngle < 0)
    {
        xzAngle += 2*PI;
    }

    // Rotate the solids
    for(std::shared_ptr<Solid> s : solids)
    {
        s->rotateAroundOwner(0, deltaXZAngle, 0);
    }
}

void Computer::arriveAtTarget()
{
    if(!futureLocations.empty())
    {
        targetLocation = futureLocations.back();
        futureLocations.pop_back();

        turnVelocityTowardTarget();
    }
}

// ==============================
//
//          AI Things
//
// ==============================

void Computer::updateTargetAngle()
{
    double newAngle = atan2(playerLocation.z - location.z, playerLocation.x - location.x);
    if(newAngle != targetAngle)
    {
        needsToRotate = true;
    }
    targetAngle = newAngle;
}
void Computer::updateCanShootTarget()
{
    double dif = angleSweptOut(xzAngle, targetAngle);
    canShootTarget = dif < PI/4 || dif > 2*PI - PI/4;
}


void Computer::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
    glBegin(GL_LINES);
    glVertex3f(location.x, location.y, location.z);
    glVertex3f(location.x + 30*cos(xzAngle), location.y, location.z + 30*sin(xzAngle));
    glEnd();
}