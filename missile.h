#ifndef PRIME_CITY_MISSILE_H
#define PRIME_CITY_MISSILE_H

#include "recPrism.h"

class Missile
{
private:
    Point location;
    double speed;
    Point velocity;
    Point rotationalVelocity;
    double radius;
    RecPrism cube;
    bool wasShotByPlayer; // true if this is a player missile, false if computer
public:
    Missile();
    Missile(Point inputLocation, double inputSpeed, Point inputVelocity, double inputRadius, bool inputWasShotByPlayer);

    // Getters
    Point getLocation() const;
    double getSpeed() const;
    Point getVelocity() const;
    double getRadius() const;
    bool getWasShotByPlayer() const;

    // Setters
    void setLocation(Point inputLocation);
    void setSpeed(double inputSpeed);
    void setVelocity(Point inputVelocity);
    void setRadius(double inputRadius);

    void draw() const;

    // Movement
    void tick();
    bool isOutOfBounds(Point playerLocation, int radius);
};

#endif //PRIME_CITY_MISSILE_H
