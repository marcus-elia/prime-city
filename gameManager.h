#ifndef PRIME_CITY_GAMEMANAGER_H
#define PRIME_CITY_GAMEMANAGER_H

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "enemy.h"
#include "player.h"
#include "perlinNoiseGenerator.h"
#include "structs.h"
#include "chunk.h"
#include "plotNetwork.h"

class GameManager
{
private:
    Player player;

    // Controls
    bool wKey, aKey, sKey, dKey, rKey, cKey;

    // Chunks
    int chunkSize;
    int plotsPerSide;
    int renderRadius;
    std::unordered_map<int, std::shared_ptr<Chunk>> allSeenChunks;
    std::vector<std::shared_ptr<Chunk>> currentChunks;
    int perlinSize;  // how many chunks before perlin repeats
    PerlinNoiseGenerator png;

    // Enemies
    std::vector<std::shared_ptr<Enemy>> enemies;

    // Path finding
    PlotNetwork network;
public:
    GameManager();
    GameManager(int inputChunkSize, int inputPlotsPerSide, int inputRenderRadius, int inputPerlinSize);

    // Getters
    Player getPlayer() const;
    bool getWKey();
    bool getAKey();
    bool getSKey();
    bool getDKey();
    bool getRKey();
    bool getCKey();

    // Setters
    void setWKey(bool input);
    void setAKey(bool input);
    void setSKey(bool input);
    void setDKey(bool input);
    void setRKey(bool input);
    void setCKey(bool input);

    // Chunks
    double getPerlinValue(Point2D p); // the value of the Perlin noise map at that point
    void updateCurrentChunks();
    // Returns a pointer to the chunk that p is in
    std::shared_ptr<Chunk> pointToChunk(Point p);

    // Path finding
    int getIDofNearestPlot(Point p);

    // Missiles
    void createMissile();
    void checkMissiles();

    // Camera
    Vector3 getCameraLocation() const;
    Vector3 getCameraLookingAt() const;
    Vector3 getCameraUp() const;

    // Mouse
    void reactToMouseMovement(double theta);
    void reactToMouseClick();

    void draw() const;
    void tick();

    // Debug
    void printPlayerBuildingDebug();

    // UI
    void drawPlayerDirection(double x, double y) const;
};

// Since % can return negatives
int mod(int a, int m);

#endif //PRIME_CITY_GAMEMANAGER_H
