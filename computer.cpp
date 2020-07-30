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
    playerVelocity = {0, 0, 0};

    missileTarget = {0, 0, 0};
    canShootTarget = false;
    needsToRotate = true;
    targetEnemy = std::experimental::nullopt;
    missileTargetVelocity = {0, 0, 0};

    // Point to GameManager's vector of enemies
    std::vector<std::shared_ptr<Enemy>>* enemies = nullptr;
}
Computer::Computer(Point inputLocation, double inputSpeed, double inputRotationSpeed,
                   std::vector<std::shared_ptr<Enemy>>* inputEnemies, int inputEnemyBlastRadius)
{
    location = inputLocation;
    speed = inputSpeed;
    velocity = {0, 0, 0};
    rotationSpeed = inputRotationSpeed;
    enemies = inputEnemies;
    enemyBlastRadius = inputEnemyBlastRadius;

    radius = 8;
    bodyHeight = 24;

    xzAngle = PI/2;
    targetAngle = PI/2;

    bodyColor = {1, 0.8, 0, 1};
    headColor = {0.42, 0.2, 0.48, 1};
    initializeSolids();

    targetLocation = location;
    playerLocation = {0, 0, 0};
    playerVelocity = {0, 0, 0};

    missileTarget = {0, 0, 0};
    canShootTarget = true;
    needsToRotate = true;
    targetEnemy = std::experimental::nullopt;
    missileTargetVelocity = {0, 0, 0};
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
    double armRadius = 4;
    double armLength = 20;
    center = {location.x + radius + armRadius, 2*bodyHeight/3, location.z + armLength/2};
    std::shared_ptr<Capsule> leftArm = std::make_shared<Capsule>(Capsule(center, {0.75, 0.45, 0, 1},
                                                                         2*armRadius, armLength, 2*armRadius, edgeColor));
    leftArm->setOwnerCenter(location);
    leftArm->rotate(PI/2, 0, 0);
    solids.push_back(leftArm);

    center = {location.x - radius - armRadius, bodyHeight/2, location.z};
    std::shared_ptr<Capsule> rightArm = std::make_shared<Capsule>(Capsule(center, headColor,
                                                                         2*armRadius, armLength, 2*armRadius, edgeColor));
    rightArm->setOwnerCenter(location);
    solids.push_back(rightArm);

    // Infinity stones
    center = {location.x + radius + armRadius, 2*bodyHeight/3 + armRadius, location.z + 3*armLength/4};
    std::shared_ptr<Ellipsoid> gem1 = std::make_shared<Ellipsoid>(Ellipsoid(center, {1, 1, 0, 1},
                                                                         3, 2, 3, edgeColor, true, 3, 6));
    gem1->setOwnerCenter(location);
    solids.push_back(gem1);

    center = {location.x + radius + 1, 2*bodyHeight/3 + armRadius - 2, location.z + 7*armLength/8};
    std::shared_ptr<Ellipsoid> gem2 = std::make_shared<Ellipsoid>(Ellipsoid(center, {0.5, 0, 0.5, 1},
                                                                            2, 2, 2, edgeColor, true, 3, 6));
    gem2->setOwnerCenter(location);
    solids.push_back(gem2);

    center = {location.x + radius + 3, 2*bodyHeight/3 + armRadius - 2, location.z + 9.5*armLength/10};
    std::shared_ptr<Ellipsoid> gem3 = std::make_shared<Ellipsoid>(Ellipsoid(center, {0.2, 0.2, 1, 1},
                                                                            2, 2, 2, edgeColor, true, 3, 6));
    gem3->setOwnerCenter(location);
    solids.push_back(gem3);

    center = {location.x + radius + armRadius*2 - 3, 2*bodyHeight/3 + armRadius - 2, location.z + 9.5*armLength/10};
    std::shared_ptr<Ellipsoid> gem4 = std::make_shared<Ellipsoid>(Ellipsoid(center, {1, 0.2, 0.2, 1},
                                                                            2, 2, 2, edgeColor, true, 3, 6));
    gem4->setOwnerCenter(location);
    solids.push_back(gem4);

    center = {location.x + radius + armRadius*2 - 1, 2*bodyHeight/3 + armRadius - 2, location.z + 7*armLength/8};
    std::shared_ptr<Ellipsoid> gem5 = std::make_shared<Ellipsoid>(Ellipsoid(center, {1.0, 0.4, 0, 1},
                                                                            2, 2, 2, edgeColor, true, 3, 6));
    gem5->setOwnerCenter(location);
    solids.push_back(gem5);

    center = {location.x + radius, 2*bodyHeight/3, location.z + 3*armLength/4};
    std::shared_ptr<Ellipsoid> gem6 = std::make_shared<Ellipsoid>(Ellipsoid(center, {0.3, 1.0, 0.3, 1},
                                                                            2, 3, 3, edgeColor, true, 3, 6));
    gem6->setOwnerCenter(location);
    solids.push_back(gem6);
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
double Computer::getMissileSpeed() const
{
    return MISSILE_SPEED;
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
void Computer::setPlayerVelocity(Point inputPlayerVelocity)
{
    playerVelocity = inputPlayerVelocity;
}
void Computer::setEnemies(std::vector<std::shared_ptr<Enemy>> *inputEnemies)
{
    enemies = inputEnemies;
}


// ===================================
//
//         Moving and Ticks
//
// ==================================
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
        move();
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
    double movingAngle = atan2(targetLocation.z - location.z, targetLocation.x - location.x);
    velocity = {speed*cos(movingAngle), 0, speed*sin(movingAngle)};
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
    double newAngle = atan2(missileTarget.z - location.z, missileTarget.x - location.x);
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

std::experimental::optional<std::shared_ptr<Enemy>> Computer::chooseBestEnemyToShootBasic()
{
    int max = 0;
    std::experimental::optional<std::shared_ptr<Enemy>> bestEnemy = std::experimental::nullopt;
    for(std::shared_ptr<Enemy> e : *enemies)
    {
        // If the number if prime and > the previous max and the enemy is close enough
        if(e->getIsPrime() && e->getNumber() > max && distance2d(location, e->getLocation()) < ENEMY_TARGET_DISTANCE)
        {
            max = e->getNumber();
            bestEnemy = e;
        }
    }
    return bestEnemy;
}

std::experimental::optional<std::shared_ptr<Enemy>> Computer::chooseBestEnemyToShootAdvanced()
{
    double max = 0; // the value of an Enemy is the prime you could get divided by the distance away it is
    std::experimental::optional<std::shared_ptr<Enemy>> bestEnemy = std::experimental::nullopt;
    for(int i = 0; i < enemies->size(); i++)
    {
        std::shared_ptr<Enemy> e = (*enemies)[i];

        // Check how far away it is. If it is at this location, just return it to avoid dividing by zero
        double distanceAway = distance2d(location, e->getLocation());
        if(distanceAway == 0)
        {
            return e;
        }

        // If the Enemy is close enough to consider shooting it
        if(distanceAway < ENEMY_TARGET_DISTANCE)
        {
            double curEnemyValue = e->getNumber() / distanceAway;
            if(e->getIsPrime() && curEnemyValue > max)
            {
                max = curEnemyValue;
                bestEnemy = e;
            }
            else // Check if the composite can create any high primes
            {
                for(int j = 0; j < enemies->size(); j++)
                {
                    if(i != j) // Don't check with itself
                    {
                        std::shared_ptr<Enemy> e2 = (*enemies)[j];
                        if(distance2d(e->getLocation(), e2->getLocation()) < enemyBlastRadius)
                        {
                            int newSum = (e->getNumber() + e2->getNumber()) % 100;
                            curEnemyValue = newSum / distanceAway;
                            if(twoDigitIsPrime(newSum) && curEnemyValue > max)
                            {
                                max = curEnemyValue;
                                bestEnemy = e;
                            }
                        }
                    }
                }
            }
        }
    }
    return bestEnemy;
}

void Computer::updateShootingTarget()
{
    // The targetEnemy will be nullopt if there is not a good enemy to shoot at
    std::experimental::optional<std::shared_ptr<Enemy>> bestEnemy = chooseBestEnemyToShootAdvanced();
    targetEnemy = bestEnemy;
    updateShootingTargetInfo();
}

void Computer::updateShootingTargetInfo()
{
    if(!targetEnemy)
    {
        missileTarget = playerLocation;
        missileTargetVelocity = playerVelocity;
    }
    else
    {
        missileTarget = (*targetEnemy)->getLocation();
        missileTargetVelocity = (*targetEnemy)->getVelocity();
    }
}

Point Computer::getActualTargetPoint() const
{
    return predictMovement(location, MISSILE_SPEED, missileTarget, missileTargetVelocity);
}


void Computer::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
}