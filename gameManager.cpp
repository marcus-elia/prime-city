#include "gameManager.h"

GameManager::GameManager()
{
    perlinSize = 10;
    png = PerlinNoiseGenerator(10, 10, 1);
    chunkSize = 512;
    plotsPerSide = 8;
    network = PlotNetwork(chunkSize, plotsPerSide);
    renderRadius = 5;
    playerPlotID = 0;
    updateCurrentChunks();

    playerScore = 0;
    enemyBodyHeight = 24;
    enemyRadius = 8;
    enemySpeed = 1.5;

    frameNumberMod90 = 0;
}
GameManager::GameManager(int inputChunkSize, int inputPlotsPerSide, int inputRenderRadius, int inputPerlinSize)
{
    chunkSize = inputChunkSize;
    plotsPerSide = inputPlotsPerSide;
    network = PlotNetwork(chunkSize, plotsPerSide);
    renderRadius = inputRenderRadius;
    perlinSize = inputPerlinSize;
    playerPlotID = 0;
    png = PerlinNoiseGenerator(perlinSize, perlinSize, 1);
    updateCurrentChunks();

    playerScore = 0;
    enemyBodyHeight = 24;
    enemyRadius = 8;
    enemySpeed = 1.5;

    frameNumberMod90 = 0;
}

void GameManager::reactToMouseMovement(double theta)
{
    player.updateAngles(theta);
    player.updateSphericalDirectionBasedOnAngles();
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::reactToMouseClick()
{
    createMissile();
}


void GameManager::draw() const
{
    // Draw in order because of transparency
    for(std::shared_ptr<Chunk> c : currentChunks)
    {
        c->draw();
    }
    for(std::shared_ptr<Enemy> e : enemies)
    {
        e->draw();
    }
    for(std::shared_ptr<Missile> m : missiles)
    {
        m->draw();
    }
    for(std::shared_ptr<Explosion> e : explosions)
    {
        e->draw();
    }
}

void GameManager::tick()
{
    // The player moves
    player.tick();

    // Update the player's plot once per second
    if(frameNumberMod90 % 30 == 0)
    {
        Vector3 newLocation = player.getLocation();
        int newID = getIDofNearestPlot({newLocation.x, newLocation.y, newLocation.z}, chunkSize, plotsPerSide);
        if(newID != playerPlotID || frameNumberMod90 == 0)
        {
            playerPlotID = newID;
            updateEnemyPathFinding();
        }
    }

    // Check for the player hitting a building
    Point2D curPlayerChunk = player.whatChunk();
    std::shared_ptr<Chunk> c = allSeenChunks[pointToInt(curPlayerChunk)];
    player.checkCollisionsAndCorrect(*c, 5);

    // If the player is entering a different chunk
    if(curPlayerChunk != player.getCurrentChunkCoords())
    {
        updateCurrentChunks();
    }

    // The enemies move
    for(std::shared_ptr<Enemy> enemy : enemies)
    {
        enemy->tick();
    }
    // Spawn and despawn enemies once every 1.5 seconds
    if(frameNumberMod90 % 45 == 0)
    {
        manageEnemies();
    }
    // The enemies look at the player
    if(frameNumberMod90 % 16 == 0)
    {
        Point playerLocation = {player.getLocation().x, 0, player.getLocation().z};
        for(std::shared_ptr<Enemy> enemy : enemies)
        {
            enemy->lookAtPlayer(playerLocation);
        }
    }

    // The missiles move
    for(std::shared_ptr<Missile> m : missiles)
    {
        m->tick();
    }
    // Check for missile collisions with buildings and enemies
    checkMissiles();


    // Explosions
    for(std::shared_ptr<Explosion> e : explosions)
    {
        e->tick();
    }
    manageExplosions();


    frameNumberMod90++;
    frameNumberMod90 %= 90;
}

Player GameManager::getPlayer() const
{
    return player;
}
bool GameManager::getWKey()
{
    return wKey;
}
bool GameManager::getAKey()
{
    return aKey;
}
bool GameManager::getSKey()
{
    return sKey;
}
bool GameManager::getDKey()
{
    return dKey;
}
bool GameManager::getRKey()
{
    return rKey;
}
bool GameManager::getCKey()
{
    return cKey;
}

void GameManager::setWKey(bool input)
{
    wKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setAKey(bool input)
{
    aKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setSKey(bool input)
{
    sKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setDKey(bool input)
{
    dKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setRKey(bool input)
{
    rKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}
void GameManager::setCKey(bool input)
{
    cKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
}

// ============================
//
//       Managing Chunks
//
// ============================
double GameManager::getPerlinValue(Point2D p)
{
    return png.getPerlinNoise()[mod(p.x, perlinSize)][mod(p.z, perlinSize)];
}

void GameManager::updateCurrentChunks()
{
    player.setCurrentChunkCoords(player.whatChunk());

    // Update the list of current chunks
    currentChunks = std::vector<std::shared_ptr<Chunk>>();
    std::vector<Point2D> chunksInRadius = getChunksAroundPointByPoint(player.getCurrentChunkCoords(), renderRadius);
    for(Point2D p : chunksInRadius)
    {
        int index = pointToInt(p);
        if(allSeenChunks.count(index) == 0) // if the chunk has never been seen before
        {
            // Create and add a new Chunk
            allSeenChunks[index] = std::make_shared<Chunk>(p, chunkSize, getPerlinValue(p));

            // Update the network with the plots from the new chunk
            std::vector<int> emptyPlotIDs = allSeenChunks[index]->getEmptyPlotIDs();
            for(int id : emptyPlotIDs)
            {
                network.addNode(PlotNode(id, chunkSize, allSeenChunks[index]->getPlotsPerSide()));
            }
        }
        currentChunks.push_back(allSeenChunks[index]);
    }
}
std::shared_ptr<Chunk> GameManager::pointToChunk(Point p)
{
    int chunkIndex = pointToInt({(int)floor(p.x / chunkSize), (int)floor(p.z / chunkSize)});
    if(allSeenChunks.count(chunkIndex) == 0)
    {
        chunkIndex = 0; // If it's out of bounds, just check the 0 chunk for now. can't hurt.
    }
    return allSeenChunks[chunkIndex];
}



// =========================
//
//      Path Finding
//
// =========================

void GameManager::updateEnemyPathFinding()
{
    for(int i = 0; i < enemies.size(); i++)
    {
        int enemyPlotID = getIDofNearestPlot(enemies[i]->getLocation(), chunkSize, plotsPerSide);
        if(enemyPlotID == playerPlotID) // If close to player, just target a random point near the player
        {
            enemies[i]->setIsCloseToPlayer(true);
        }
        else // Otherwise, path find
        {
            enemies[i]->setIsCloseToPlayer(false);
            std::vector<Point> path = network.getClippedPathPoints(enemyPlotID, playerPlotID, ENEMY_BFS_SEARCH_DEPTH);
            // If the path will cause the enemy's first movement will be within the plot it's currently in,
            // then pop that location off
            if(!path.empty() && enemyPlotID == getIDofNearestPlot(path.back(), chunkSize, plotsPerSide))
            {
                path.pop_back();
            }
            enemies[i]->setFutureLocations(path);
        }
    }
}

// =========================
//
//         Enemies
//
// =========================
Point GameManager::getRandomOpenLocation()
{
    int randIndex = rand() % currentChunks.size();
    std::vector<int> emptyPlots = currentChunks[randIndex]->getEmptyPlotIDs();
    randIndex = rand() % emptyPlots.size();
    int plot = emptyPlots[randIndex];
    if(!network.hasNode(plot))
    {
        int a = 1;
    }
    return getPlotCenterFromID(plot, chunkSize, plotsPerSide);
}
void GameManager::createRandomEnemy()
{
    Point location = getRandomOpenLocation();
    location.y = enemyBodyHeight/2;
    enemies.push_back(std::make_shared<Enemy>(location, enemyBodyHeight, enemyRadius,
            enemySpeed,  0, rand() % 100));
}
void  GameManager::manageEnemies()
{
    // First, iterate through and remove enemies that are too far away
    int L = enemies.size();
    int i = 0;
    while(i < L)
    {
        std::shared_ptr<Enemy> e = enemies[i];
        Point playerLocation = {player.getLocation().x, player.getLocation().y, player.getLocation().z};

        if(distance2d(playerLocation, e->getLocation()) > renderRadius*chunkSize)
        {
            enemies.erase(enemies.begin() + i);
            L -= 1;
            i--;
        }
        i++;
    }
    // If we need more, make more
    if(enemies.size() < MAX_NUM_ENEMIES)
    {
        createRandomEnemy();
    }
}
void GameManager::createEnemyExplosion(std::shared_ptr<Enemy> e)
{
    Point loc = e->getLocation();
    double rad = e->getRadius();
    int lifeTime = 60;
    RGBAcolor color = e->getBodyColor();
    double deltaRad = 2;
    explosions.push_back(std::make_shared<Explosion>(loc, rad, lifeTime, color, deltaRad, e->getNumber()));
}

// =========================
//
//        Missiles
//
// =========================
void GameManager::createMissile()
{
    Point location = {player.getLocation().x, player.getLocation().y, player.getLocation().z};
    Point velocity = {player.getLookingAt().x - location.x,
                      player.getLookingAt().y - location.y,
                      player.getLookingAt().z - location.z};
    missiles.push_back(std::make_shared<Missile>(Missile(location, 5, velocity, 5, true, PLAYER_MISSILE_COLOR)));
}
void GameManager::checkMissiles()
{
    int L = missiles.size();
    int i = 0;
    while(i < L)
    {
        std::shared_ptr<Missile> m = missiles[i];
        Point playerLocation = {player.getLocation().x, player.getLocation().y, player.getLocation().z};

        // If the missile went out of bounds without hitting anything, remove it
        if(m->isOutOfBounds(playerLocation, renderRadius*chunkSize))
        {
            createMissileExplosion(m); // Make an explosion where the missile hit
            missiles.erase(missiles.begin() + i);
            L -= 1;
            i--;
        }
        // Check if the missile is hitting something
        else
        {
            // First, check for Buildings
            std::shared_ptr<Chunk> c = pointToChunk(m->getLocation());
            for(Building &b : c->getBuildings())
            {
                if(b.correctCollision(m->getLocation(), m->getRadius()))
                {
                    createMissileExplosion(m); // Make an explosion where the missile hit
                    missiles.erase(missiles.begin() + i);
                    L -= 1;
                    i--;
                    break;
                }
            }
            // Then check for Enemies
            for(int j = 0; j < enemies.size(); j++)
            {
                std::shared_ptr<Enemy> enemy = enemies[j];
                if(enemy->isHitByMissile(m->getLocation(), m->getRadius()))
                {
                    if(enemy->getIsPrime()) // Give the player points if the number was prime
                    {
                        playerScore += enemy->getNumber();
                        createMissileExplosion(m); // Make an explosion where the missile hit
                    }
                    else   // If composite, make an enemy explosion
                    {
                        createEnemyExplosion(enemy);
                    }
                    missiles.erase(missiles.begin() + i);
                    L -= 1;
                    i--;

                    enemies.erase(enemies.begin() + j);
                    break;
                }
            }
        }
        i++;
    }
}



// ===========================
//
//         Explosions
//
// ===========================
void GameManager::createMissileExplosion(std::shared_ptr<Missile> m)
{
    Point loc = m->getLocation();
    double rad = m->getRadius();
    int lifeTime = 60;
    RGBAcolor color = m->getCoreColor();
    double deltaRad = 0.5;
    explosions.push_back(std::make_shared<Explosion>(loc, rad, lifeTime, color, deltaRad));
}
void GameManager::manageExplosions()
{
    int L = explosions.size();
    int i = 0;

    // Iterate through the explosions
    // Delete any that are done and
    // check for enemies in the blast radius
    while(i < L)
    {
        std::shared_ptr<Explosion> e = explosions[i];
        if(e->isDone())
        {
            explosions.erase(explosions.begin() + i);
            L -= 1;
            i--;
            break;
        }
        if(e->getNumber())
        {
            checkExplosionForEnemies(e);
        }
        i++;
    }
}
void GameManager::checkExplosionForEnemies(std::shared_ptr<Explosion> ex)
{
    for(std::shared_ptr<Enemy> enemy : enemies)
    {
        if(!enemy->containsExplosion(ex) && ex->containsPoint(enemy->getLocation()))
        {
            enemy->setNumber((enemy->getNumber() + ex->getNumber().value()) % 100);
            enemy->addRecentExplosion(ex);
        }
    }
}


// Camera
Vector3 GameManager::getCameraLocation() const
{
    return player.getLocation();
}
Vector3 GameManager::getCameraLookingAt() const
{
    return player.getLookingAt();
}
Vector3 GameManager::getCameraUp() const
{
    return player.getUp();
}

int mod(int a, int m)
{
    int x = a % m;
    if(x < 0)
    {
        return x + m;
    }
    return x;
}

void GameManager::printPlayerBuildingDebug()
{
    Vector3 v = player.getLocation();
    //std::cout << std::endl<< "Player location: " << v.x << "," << v.y << "," << v.z << std::endl;
    //Point2D curPlayerChunk = player.whatChunk();
    //std::shared_ptr<Chunk> c = allSeenChunks[pointToInt(curPlayerChunk)];
    std::cout << "Player plot " << playerPlotID << std::endl;
    std::cout << "Enemy plot " << getIDofNearestPlot(enemies[0]->getLocation(), chunkSize, plotsPerSide) << std::endl;
    /*for(Building &b : c->getBuildings())
    {
        for(std::shared_ptr<Solid> s : b.getSolids())
        {
            s->printDebugStats();
        }
        std::cout<<"End of Building" << std::endl;
    }*/
}

void GameManager::drawPlayerDirection(double x, double y) const
{
    int compassRadius = 25;

    // Draw the text
    glColor4f(0.0, 0.0, 0.0, 1.0);
    std::string direction = player.getDirectionString();
    glRasterPos2i(x - (4 * direction.length()), y - 3);
    for (const char &letter : direction)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
    }

    // Draw the red line
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    double theta = player.getXZAngle();
    glVertex2f(x + compassRadius*cos(theta), y - compassRadius*sin(theta));
    glEnd();

    // Draw the circle
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
    int smoothness = 24;
    glVertex2f(x, y); // center
    for(int i = 0; i < smoothness + 1; i++)
    {
        glVertex2f(x + compassRadius*cos(i*2*PI/smoothness), y + compassRadius*sin(i*2*PI/smoothness));
    }
    glEnd();
}
void GameManager::displayScores() const
{
    glColor4f(0.0, 0.0, 0.0, 1.0);
    std::string score = "Player: " + std::to_string(playerScore);
    glRasterPos2i(10 + (4 * score.length()), 25);
    for (const char &letter : score)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
    }
}