#ifndef PRIME_CITY_ENEMY_H
#define PRIME_CITY_ENEMY_H

#include "structs.h"
#include "graphics.h"
#include "solid.h"
#include "cylinder.h"
#include "ellipsoid.h"
#include "capsule.h"

// Physically, the Enemy is a sphere on top of a capsule. The capsule and
// the sphere have the same radius. The y-value of location is the center of
// the capsule.


class Enemy
{
private:
    Point location;
    double bodyHeight;
    double radius;
    double speed;
    double rotationSpeed;
    double xzAngle;
    double yAngle;

    // The Point this Enemy is currently moving toward
    // (not the ultimate goal)
    Point targetLocation;
    // The reverse-ordered list of points
    std::vector<Point> futureLocations;

    Vector3 velocity; // current x y and z velocity

    int number;

    std::vector<std::shared_ptr<Solid>> solids;

    //constexpr const static
    RGBAcolor bodyColor = {0.0, 0.5, 1.0, 1.0};
    //constexpr const static
    RGBAcolor headColor = {0.8, 0.8, 0.8, 0.5};
public:
    Enemy();
    Enemy(Point inputLocation, double inputBodyHeight, double inputRadius,
            double inputSpeed, double inputRotationSpeed, int inputNumber);

    void initializeSolids();

    Point getLocation() const;

    void setFutureLocations(std::vector<Point> inputFutureLocations);

    // Set angle and velocity to face toward the targetLocation
    void turnTowardTarget();

    // When the enemy reaches the target location, start moving
    // toward the next target
    void arriveAtTarget();

    void tick();

    void draw() const;
};

#endif //PRIME_CITY_ENEMY_H
