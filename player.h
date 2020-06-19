#ifndef RANDOM_3D_CITY_PLAYER_H
#define RANDOM_3D_CITY_PLAYER_H

#include "graphics.h"
#include "chunk.h"

class Player
{
private:
    Vector3 location;
    Vector3 lookingAt;
    Vector3 up;
    Vector3 sphericalDirection; // The direction the player is facing in spherical coordinates
                                // Must be on the unit sphere

    double xzAngle; // Where the player is looking in the xz-plane
    double yAngle;  // How much the player is looking up (in [-Pi, Pi])

    double speed;     // how fast the player can move
    Vector3 velocity; // current x y and z velocity

    double sensitivity; // turning speed for mouse movement

    int chunkSize;
    Point2D currentChunkCoords; // which chunk the player is in
public:
    Player();
    Player(Vector3 inputLocation, Vector3 inputLookingAt, Vector3 inputUp, double inputSpeed, int inputChunkSize);

    // Getters
    Vector3 getLocation() const;
    Vector3 getLookingAt() const;
    Vector3 getUp() const;
    double getSpeed() const;
    double getXZAngle() const;
    double getYAngle() const;
    Point2D getCurrentChunkCoords() const;
    int getCurrentChunkInt() const;

    // Setters
    void setLocation(Vector3 inputLocation);
    void setLookingAt(Vector3 inputLookingAt);
    void setUp(Vector3 inputUp);
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
};

#endif //RANDOM_3D_CITY_PLAYER_H
