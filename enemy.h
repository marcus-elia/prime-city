#ifndef PRIME_CITY_ENEMY_H
#define PRIME_CITY_ENEMY_H

#include "structs.h"
#include "graphics.h"
#include "solid.h"
#include "cylinder.h"
#include "ellipsoid.h"

// Physically, the Enemy is a sphere on top of a capsule. The capsule and
// the sphere have the same radius. The y-value of location is the center of
// the capsule.


class Enemy
{
private:
    Vector3 location;
    double bodyHeight;
    double radius;
    double speed;
    double rotationSpeed;
    double xzAngle;
    double yAngle;

    Vector3 velocity; // current x y and z velocity

    int number;

    std::vector<std::shared_ptr<Solid>> solids;

    constexpr const static RGBAcolor bodyColor = {0.0, 0.5, 1.0, 1.0};
    constexpr const static RGBAcolor headColor = {0.8, 0.8, 0.8, 0.5};
public:
    Enemy();
    Enemy(Vector3 inputLocation, double inputBodyHeight, double inputRadius,
            double inputSpeed, double inputRotationSpeed, int inputNumber);

    void initializeSolids();

    void draw() const;
};

#endif //PRIME_CITY_ENEMY_H
