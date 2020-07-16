#ifndef PRIME_CITY_GAMEMANAGER_H
#define PRIME_CITY_GAMEMANAGER_H

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "enemy.h"
#include "player.h"
#include "missile.h"
#include "perlinNoiseGenerator.h"
#include "structs.h"
#include "chunk.h"
#include "plotNetwork.h"
#include "mathHelper.h"
#include "explosion.h"
#include "computer.h"

class GameManager
{
private:
    // The Player
    Player player;
    int playerScore;

    // Controls
    bool wKey, aKey, sKey, dKey, rKey, cKey;

    // The Computer
    Computer computer;
    int computerScore;

    // Chunks
    int chunkSize;
    int plotsPerSide;
    int renderRadius;
    std::unordered_map<int, std::shared_ptr<Chunk>> allSeenChunks;
    std::vector<std::shared_ptr<Chunk>> currentChunks;
    int perlinSize;  // how many chunks before perlin repeats
    PerlinNoiseGenerator png;

    // Timer
    int frameNumberMod90;
    int ticksSinceLastPlayerMissile;
    int ticksSinceLastComputerMissile;

    // Enemies
    double enemyBodyHeight;
    double enemyRadius;
    double enemySpeed;
    std::vector<std::shared_ptr<Enemy>> enemies;
    const int MAX_NUM_ENEMIES = 10;

    // Missiles
    RGBAcolor PLAYER_MISSILE_COLOR = {1, 0, 0, 1};
    std::vector<std::shared_ptr<Missile>> missiles;
    const int PLAYER_MISSILE_COOLDOWN = 15;
    const int COMPUTER_MISSILE_COOLDOWN = 15;

    // Explosions
    std::vector<std::shared_ptr<Explosion>> explosions;

    // Path finding
    PlotNetwork network;
    const int ENEMY_BFS_SEARCH_DEPTH = 6;
    int playerPlotID; // the plot the player is currently in

    // UI
    double cursorAlpha; // The cursor will be opaque when reloaded
public:
    GameManager();
    GameManager(int inputChunkSize, int inputPlotsPerSide, int inputRenderRadius, int inputPerlinSize);

    // Getters
    Player getPlayer() const;
    bool getWKey() const;
    bool getAKey() const;
    bool getSKey() const;
    bool getDKey() const;
    bool getRKey() const;
    bool getCKey() const;
    double getCursorAlpha() const;

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
    void updateEnemyPathFinding();
    void updateComputerPathFinding();

    // Enemies
    Point getRandomOpenLocation();  // Returns the center of an empty plot
    void manageEnemies(); // create new enemies if needed, despawn if too far away
    void createRandomEnemy();
    void createEnemyExplosion(std::shared_ptr<Enemy> e);

    // Missiles
    void createPlayerMissile();
    void createComputerMissile();
    void checkMissiles();

    // Explosions
    void createMissileExplosion(std::shared_ptr<Missile> m);
    void manageExplosions();
    void checkExplosionForEnemies(std::shared_ptr<Explosion> ex);

    // Camera
    Point getCameraLocation() const;
    Point getCameraLookingAt() const;
    Point getCameraUp() const;

    // Mouse
    void reactToMouseMovement(double theta);
    void reactToMouseClick();

    void draw() const;
    void tick();

    // Debug
    void printPlayerBuildingDebug();

    // UI
    void drawPlayerDirection(double x, double y) const;
    void displayScores() const;
};

// Since % can return negatives
int mod(int a, int m);

#endif //PRIME_CITY_GAMEMANAGER_H
