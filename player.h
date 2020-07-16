#ifndef RANDOM_3D_CITY_PLAYER_H
#define RANDOM_3D_CITY_PLAYER_H

#include "graphics.h"
#include "chunk.h"

class Player
{
private:
    Point location;
    Point lookingAt;
    Point up;
    Point sphericalDirection; // The direction the player is facing in spherical coordinates
                                // Must be on the unit sphere

    double xzAngle; // Where the player is looking in the xz-plane
    double yAngle;  // How much the player is looking up (in [-Pi, Pi])

    double speed;     // how fast the player can move
    Point velocity; // current x y and z velocity

    double sensitivity; // turning speed for mouse movement

    int chunkSize;
    Point2D currentChunkCoords; // which chunk the player is in

    double height;
    double radius;
public:
    Player();
    Player(Point inputLocation, Point inputLookingAt, Point inputUp, double inputSpeed, int inputChunkSize);

    // Getters
    Point getLocation() const;
    Point getLookingAt() const;
    Point getUp() const;
    double getSpeed() const;
    double getXZAngle() const;
    double getYAngle() const;
    Point2D getCurrentChunkCoords() const;
    int getCurrentChunkInt() const;
    double getHeight() const;
    double getRadius() const;

    bool isHitByMissile(Point missileLoc, double missileRadius) const;

    // Setters
    void setLocation(Point inputLocation);
    void setLookingAt(Point inputLookingAt);
    void setUp(Point inputUp);
    void setSpeed(double inputSpeed);
    void setXZAngle(double inputXZAngle);
    void setYAngle(double inputYAngle);
    void setCurrentChunkCoords(Point2D input);

    // Movement

    // Based on which keys are pressed, set the velocity
    void setVelocity(bool wKey, bool aKey, bool sKey, bool dKey, bool rKey, bool cKey);

    // Update the xzAngle and yAngle based on theta resulting from a mouse movement
    void updateAngles(double theta);

    // Use xzAngle, yAngle, and location to determine the spherical direction.
    void updateSphericalDirectionBasedOnAngles();


    // Chunks

    // Returns the Point2D of the chunk underneath where the player is
    Point2D whatChunk() const;

    void tick();

    // Check every Building in the given Chunk to see if we are hitting it,
    // and move according to what the Building says if we are
    void checkCollisionsAndCorrect(Chunk &c, int buffer);

    // UI
    std::string getDirectionString() const;
};

#endif //RANDOM_3D_CITY_PLAYER_H
