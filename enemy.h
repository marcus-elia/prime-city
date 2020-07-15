#ifndef PRIME_CITY_ENEMY_H
#define PRIME_CITY_ENEMY_H

#include "structs.h"
#include "graphics.h"
#include "solid.h"
#include "cylinder.h"
#include "ellipsoid.h"
#include "capsule.h"
#include "digitalNumber.h"
#include "explosion.h"

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
    bool isCloseToPlayer;

    int chunkSize;
    int plotsPerSide;

    Point velocity; // current x y and z velocity

    int number;
    DigitalNumber dn;

    std::vector<std::shared_ptr<Solid>> solids;

    //constexpr const static
    RGBAcolor bodyColor = {0.0, 0.5, 1.0, 1.0};
    //constexpr const static
    RGBAcolor headColor = {0.8, 0.8, 0.8, 0.5};

    // Must store recent explosions that hit this, so they only get added once
    std::vector<std::shared_ptr<Explosion>> recentExplosions;
    int ticksSinceLastExplosion;
    const static int CLEAR_EXPLOSIONS_TIME = 101;
public:
    Enemy();
    Enemy(Point inputLocation, double inputBodyHeight, double inputRadius,
            double inputSpeed, double inputRotationSpeed, int inputNumber);

    void initializeSolids();

    // Getters
    Point getLocation() const;
    bool getIsPrime() const;
    int getNumber() const;
    double getRadius() const;
    RGBAcolor getBodyColor() const;

    // Setters
    void setNumber(int newNumber);
    void setPlotsPerSide(int inputPlotsPerSide);
    void setChunkSize(int chunkSize);
    void setIsCloseToPlayer(bool input);

    // Explosions
    void addRecentExplosion(std::shared_ptr<Explosion> ex);
    bool containsExplosion(std::shared_ptr<Explosion> ex) const;

    // Rotate the digital number to face the player
    void lookAtPlayer(Point playerLocation);

    void setFutureLocations(std::vector<Point> inputFutureLocations);

    // Set angle and velocity to face toward the targetLocation
    void turnTowardTarget();

    void move();

    // When the enemy reaches the target location, start moving
    // toward the next target
    void arriveAtTarget();

    void tick();

    void draw() const;

    // Returns true if the missile is currently colliding with this
    bool isHitByMissile(Point missileLoc, double missileRadius);
};

#endif //PRIME_CITY_ENEMY_H
