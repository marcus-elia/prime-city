#ifndef PRIME_CITY_MOVABLECOMPONENT_H
#define PRIME_CITY_MOVABLECOMPONENT_H

/*
 * A MovableComponent is a shape that is part of a bigger object
 * (like a wheel on a car). It keeps track of its owner's center
 * so it can rotate around it.
 */

#include "movable.h"

class MovableComponent : public Movable
{
protected:
    // The center point that this will rotate around
    Point ownerCenter;
public:
    MovableComponent();
    MovableComponent(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity);
    MovableComponent(Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
                     Point inputOwnerCenter);

    Point getOwnerCenter() const;

    void setOwnerCenter(Point inputOwnerCenter);
    void moveOwnerCenter(double deltaX, double deltaY, double deltaZ);

    virtual void rotateAroundOwner(double thetaX, double thetaY, double thetaZ) = 0;
};

#endif //PRIME_CITY_MOVABLECOMPONENT_H
