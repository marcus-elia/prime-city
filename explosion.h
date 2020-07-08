#ifndef PRIME_CITY_EXPLOSION_H
#define PRIME_CITY_EXPLOSION_H

#include "ellipsoid.h"

class Explosion
{
private:
    Point location;
    double currentRadius;
    int lifeTime;
    int age;
    RGBAcolor currentColor;

    Ellipsoid sphere;

    double deltaRadius; // How quickly it expands
    double deltaAlpha;  // How quickly it fades

    // If the explosion was caused by hitting a composite number, it stores that
    // number in case the explosion hits any Enemies
    std::experimental::optional<int> number;
public:
    Explosion();
    // If inputNumber is negative, then number = nullopt
    Explosion(Point inputLocation, double inputRadius, int inputLifeTime, RGBAcolor inputColor, double inputDeltaRadius,
            int inputNumber=-1);

    void tick();
    void draw() const;

    std::experimental::optional<int> getNumber() const;
    bool isDone() const;
    // Returns true if the point is in the sphere
    bool containsPoint(Point p) const;
};

#endif //PRIME_CITY_EXPLOSION_H
