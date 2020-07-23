#ifndef PRIME_CITY_COMPUTER_H
#define PRIME_CITY_COMPUTER_H

#include "structs.h"
#include "mathHelper.h"
#include "ellipsoid.h"
#include "capsule.h"
#include "graphics.h"
#include "enemy.h"
#include <vector>
#include <memory>

class Computer
{
protected:
    Point location;
    double speed;
    Point velocity;
    double xzAngle;
    double targetAngle;
    double rotationSpeed;

    double radius;
    double bodyHeight;

    std::vector<std::shared_ptr<Solid>> solids;
    RGBAcolor bodyColor;
    RGBAcolor headColor;

    // The Point this is currently moving toward
    // (not the ultimate goal)
    Point targetLocation;
    // The reverse-ordered list of points
    std::vector<Point> futureLocations;
    Point playerLocation;

    // Where it wants to shoot
    Point missileTarget;
    bool canShootTarget;
    bool needsToRotate;

    // Point to GameManager's vector of enemies
    std::vector<std::shared_ptr<Enemy>>* enemies;
    int enemyBlastRadius;
    const double ENEMY_TARGET_DISTANCE = 1024; // From how far away will the AI go after a prime number?
public:
    Computer();
    Computer(Point inputLocation, double inputSpeed, double inputRotationSpeed,
            std::vector<std::shared_ptr<Enemy>>* inputEnemies, int inputEnemyBlastRadius);

    void initializeSolids();

    // Getters
    Point getLocation() const;
    double getRadius() const;
    RGBAcolor getHeadColor() const;
    RGBAcolor getBodyColor() const;
    bool getCanShootTarget() const;
    Point getMissileTarget() const;

    bool isHitByMissile(Point missileLoc, double missileRadius) const;

    // Setters
    void setFutureLocations(std::vector<Point> inputFutureLocations);
    void setPlayerLocation(Point inputPlayerLocation);

    // Movement and Ticks
    void tick();
    void turnVelocityTowardTarget();
    void move();
    void turn();
    void rotate(double deltaXZAngle);
    void arriveAtTarget();

    // AI
    void updateTargetAngle();

    // Computer can fire a missile toward the target if the desired
    // angle is within a 90 degree field of vision
    void updateCanShootTarget();

    // If there is a prime that can be shot, aim for the highest one
    std::experimental::optional<std::shared_ptr<Enemy>> chooseBestEnemyToShootBasic();

    // If there is a prime that can be shot or if a composite can create a higher prime,
    // return the best enemy to shoot at
    std::experimental::optional<std::shared_ptr<Enemy>> chooseBestEnemyToShootAdvanced();

    void draw() const;

};

#endif //PRIME_CITY_COMPUTER_H
