#include "gameManager.h"

GameManager::GameManager()
{
    screenWidth = 800;
    screenHeight = 500;
    perlinSize = 10;
    png = PerlinNoiseGenerator(10, 10, 1);
    chunkSize = 512;
    plotsPerSide = 8;
    network = PlotNetwork(chunkSize, plotsPerSide);
    renderRadius = 5;
    playerPlotID = 0;
    updateCurrentChunks();

    playerScore = 0;
    computerScore = 0;
    enemyBodyHeight = 24;
    enemyRadius = 8;
    enemySpeed = 1.5;
    currentStatus = Intro;

    frameNumberMod90 = 0;
    ticksSinceLastPlayerMissile = PLAYER_MISSILE_COOLDOWN;
    cursorAlpha = 1.0;

    playButton = Button(screenWidth/2, screenHeight/2, 128, 64, 16, "Play", {0, 0, 0.7, 1}, {1,1,1,1}, {0, .2, 1, 1});
    playAgainButton = Button(screenWidth/2, screenHeight/2, 128, 64, 16, "Play Again", {0, 0, 0.7, 1}, {1,1,1,1}, {0, .2, 1, 1});
    quitButton = Button(screenWidth/2, screenHeight/2 - 64, 128, 64, 16, "Quit", {0.7, 0, 0, 1}, {1,1,1,1}, {1, 0.2, 0, 1});

    computer = Computer({96, 12, 0}, 2, 0.1, &enemies, ENEMY_BLAST_RADIUS);
}
GameManager::GameManager(int inputScreenWidth, int inputScreenHeight, int inputChunkSize,
        int inputPlotsPerSide, int inputRenderRadius, int inputPerlinSize)
{
    screenWidth = inputScreenWidth;
    screenHeight = inputScreenHeight;
    chunkSize = inputChunkSize;
    plotsPerSide = inputPlotsPerSide;
    network = PlotNetwork(chunkSize, plotsPerSide);
    renderRadius = inputRenderRadius;
    perlinSize = inputPerlinSize;
    playerPlotID = 0;
    png = PerlinNoiseGenerator(perlinSize, perlinSize, 1);
    updateCurrentChunks();

    playerScore = 0;
    computerScore = 0;
    enemyBodyHeight = 24;
    enemyRadius = 8;
    enemySpeed = 1.5;
    currentStatus = Intro;

    playButton = Button(screenWidth/2, screenHeight/2, 128, 64, 16, "Play", {0, 0, 0.7, 1}, {1,1,1,1}, {0, .2, 1, 1});
    playAgainButton = Button(screenWidth/2, screenHeight/2, 128, 64, 16, "Play Again", {0, 0, 0.7, 1}, {1,1,1,1}, {0, .2, 1, 1});
    quitButton = Button(screenWidth/2, screenHeight/2 - 64, 128, 64, 16, "Quit", {0.7, 0, 0, 1}, {1,1,1,1}, {1, 0.2, 0, 1});


    frameNumberMod90 = 0;
    ticksSinceLastPlayerMissile = PLAYER_MISSILE_COOLDOWN;
    cursorAlpha = 1.0;

    computer = Computer({96, 12, 0}, 2, 0.1, &enemies, ENEMY_BLAST_RADIUS);
}

void GameManager::reactToMouseMovement(int mx, int my, double theta)
{
    if(currentStatus == Intro)
    {
        if(playButton.containsPoint(mx, my))
        {
            playButton.setIsHighlighted(true);
        }
        else
        {
            playButton.setIsHighlighted(false);
        }

        if(quitButton.containsPoint(mx, my))
        {
            quitButton.setIsHighlighted(true);
        }
        else
        {
            quitButton.setIsHighlighted(false);
        }
    }
    else if(currentStatus == Playing)
    {
        player.updateAngles(theta);
        player.updateSphericalDirectionBasedOnAngles();
        player.setVelocity(wKey, aKey, sKey, dKey, rKey, cKey);
    }
    else if(currentStatus == End)
    {
        if(playAgainButton.containsPoint(mx, my))
        {
            playAgainButton.setIsHighlighted(true);
        }
        else
        {
            playAgainButton.setIsHighlighted(false);
        }

        if(quitButton.containsPoint(mx, my))
        {
            quitButton.setIsHighlighted(true);
        }
        else
        {
            quitButton.setIsHighlighted(false);
        }
    }
}
void GameManager::reactToMouseClick(int mx, int my)
{
    if(currentStatus == Intro)
    {
        if(playButton.containsPoint(mx, my))
        {
            currentStatus = Playing;
        }
        else if(quitButton.containsPoint(mx, my))
        {
            closeWindow = true;
        }
    }
    else if(currentStatus == Playing)
    {
        if(ticksSinceLastPlayerMissile == PLAYER_MISSILE_COOLDOWN)
        {
            createPlayerMissile();
            ticksSinceLastPlayerMissile = 0;
        }
    }
    else if(currentStatus == End)
    {
        if(playAgainButton.containsPoint(mx, my))
        {
            resetGame();
        }
        else if(quitButton.containsPoint(mx, my))
        {
            closeWindow = true;
        }
    }
}


void GameManager::draw() const
{
    if(currentStatus == Playing)
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
        computer.draw();
        for(std::shared_ptr<Missile> m : missiles)
        {
            m->draw();
        }
        for(std::shared_ptr<Explosion> e : explosions)
        {
            e->draw();
        }
    }
}


// ===========================
//
//           Tick
//
// ===========================

void GameManager::tick()
{
    if(currentStatus == Playing)
    {
        playerTick();

        enemyTick();

        missileTick();

        explosionTick();

        computerTick();

        // Time things
        frameNumberMod90++;
        frameNumberMod90 %= 90;

        checkForGameEnd();
    }
}

void GameManager::playerTick()
{
    // The player moves
    player.tick();

    // Check for the player hitting a building
    Point2D curPlayerChunk = player.whatChunk();
    std::shared_ptr<Chunk> c = allSeenChunks[pointToInt(curPlayerChunk)];
    player.checkCollisionsAndCorrect(*c, 5);

    // If the player is entering a different chunk
    if(curPlayerChunk != player.getCurrentChunkCoords())
    {
        updateCurrentChunks();
    }

    // Update the player's plot once per second
    if(frameNumberMod90 % 30 == 0)
    {
        int newID = getIDofNearestPlot(player.getLocation(), chunkSize, plotsPerSide);
        if(newID != playerPlotID || frameNumberMod90 == 0)
        {
            playerPlotID = newID;
            updateEnemyPathFinding();
        }
    }
    // Update the player's missile cooldown
    if(ticksSinceLastPlayerMissile < PLAYER_MISSILE_COOLDOWN)
    {
        ticksSinceLastPlayerMissile++;
        cursorAlpha = (double)ticksSinceLastPlayerMissile/PLAYER_MISSILE_COOLDOWN;
    }
}
void GameManager::enemyTick()
{
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
        for(std::shared_ptr<Enemy> enemy : enemies)
        {
            enemy->lookAtPlayer(player.getLocation());
        }
    }
}
void GameManager::missileTick()
{
    // The missiles move
    for(std::shared_ptr<Missile> m : missiles)
    {
        m->tick();
    }
    // Check for missile collisions with buildings and enemies
    checkMissiles();
}
void GameManager::computerTick()
{
    // The computer moves
    computer.tick();
    if(frameNumberMod90 % 45 == 0)
    {
        updateComputerPathFinding();
        if(ticksSinceLastComputerMissile == COMPUTER_MISSILE_COOLDOWN && computer.getCanShootTarget())
        {
            createComputerMissile();
        }
    }
    if(frameNumberMod90 % COMPUTER_MISSILE_COOLDOWN == 1)
    {
        computer.updateShootingTarget();
    }
    if(frameNumberMod90 % 9 == 0)
    {
        computer.setPlayerLocation(player.getLocation());
        computer.setPlayerVelocity(player.getVelocity());
        computer.updateShootingTargetInfo();
    }
    // Update the computer's missile cooldown
    if(ticksSinceLastComputerMissile < COMPUTER_MISSILE_COOLDOWN)
    {
        ticksSinceLastComputerMissile++;
    }
}
void GameManager::explosionTick()
{
    for(std::shared_ptr<Explosion> e : explosions)
    {
        e->tick();
    }
    manageExplosions();
}

// ===========================
//
//          Getters
//
// ===========================

Player GameManager::getPlayer() const
{
    return player;
}
bool GameManager::getWKey() const
{
    return wKey;
}
bool GameManager::getAKey() const
{
    return aKey;
}
bool GameManager::getSKey() const
{
    return sKey;
}
bool GameManager::getDKey() const
{
    return dKey;
}
bool GameManager::getRKey() const
{
    return rKey;
}
bool GameManager::getCKey() const
{
    return cKey;
}
double GameManager::getCursorAlpha() const
{
    return cursorAlpha;
}
GameStatus GameManager::getCurrentStatus() const
{
    return currentStatus;
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
bool GameManager::getCloseWindow() const
{
    return closeWindow;
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
void GameManager::updateComputerPathFinding()
{
    int computerPlotID = getIDofNearestPlot(computer.getLocation(), chunkSize, plotsPerSide);
    int targetPlotID = getIDofNearestPlot(computer.getMissileTarget(), chunkSize, plotsPerSide);
    std::vector<Point> path = network.getClippedPathPoints(computerPlotID, targetPlotID, ENEMY_BFS_SEARCH_DEPTH);
    computer.setFutureLocations(path);
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

        if(distance2d(player.getLocation(), e->getLocation()) > renderRadius*chunkSize)
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

// =========================
//
//        Missiles
//
// =========================
void GameManager::createPlayerMissile()
{
    Point location = player.getLocation();
    Point velocity = {player.getLookingAt().x - location.x,
                      player.getLookingAt().y - location.y,
                      player.getLookingAt().z - location.z};
    missiles.push_back(std::make_shared<Missile>(Missile(location, 5, velocity, 5, true, PLAYER_MISSILE_COLOR)));
}
void GameManager::createComputerMissile()
{
    Point location = computer.getLocation();
    Point target = computer.getActualTargetPoint();
    Point velocity = {target.x - location.x,
                      0, // Shoot level
                      target.z - location.z};
    missiles.push_back(std::make_shared<Missile>(Missile(location, 5, velocity, 5, false, computer.getHeadColor())));
    ticksSinceLastComputerMissile = 0;
}
void GameManager::checkMissiles()
{
    int L = missiles.size();
    int i = 0;
    while(i < L)
    {
        std::shared_ptr<Missile> m = missiles[i];
        Point playerLocation = player.getLocation();

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
            bool hitSomething = false;
            // First, check for Buildings
            std::shared_ptr<Chunk> c = pointToChunk(m->getLocation());
            for(Building &b : c->getBuildings())
            {
                if(b.correctCollision(m->getLocation(), m->getRadius()))
                {
                    hitSomething = true;
                    createMissileExplosion(m); // Make an explosion where the missile hit
                    missiles.erase(missiles.begin() + i);
                    L -= 1;
                    i--;
                    break;
                }
            }
            if(hitSomething)
            {
                i++;
                continue;
            }

            // Then check for Enemies
            for(int j = 0; j < enemies.size(); j++)
            {
                std::shared_ptr<Enemy> enemy = enemies[j];
                if(enemy->isHitByMissile(m->getLocation(), m->getRadius()))
                {
                    hitSomething = true;
                    if(enemy->getIsPrime()) // Give the player points if the number was prime
                    {
                        if(m->getWasShotByPlayer())
                        {
                            playerScore += enemy->getNumber();
                        }
                        else
                        {
                            computerScore += enemy->getNumber();
                        }
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
            if(hitSomething)
            {
                i++;
                continue;
            }

            // Then check the player if it's a computer missile
            if(!m->getWasShotByPlayer() && player.isHitByMissile(m->getLocation(), m->getRadius()))
            {
                hitSomething = true;
                createMissileExplosion(m);
                missiles.erase(missiles.begin() + i);
                L -= 1;
                i--;
                playerScore--;
            }
            if(hitSomething)
            {
                i++;
                continue;
            }

            // And check the computer if it's a player missile
            if(m->getWasShotByPlayer() && computer.isHitByMissile(m->getLocation(), m->getRadius()))
            {
                hitSomething = true;
                createMissileExplosion(m);
                missiles.erase(missiles.begin() + i);
                L -= 1;
                i--;
                computerScore--;
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
void GameManager::createEnemyExplosion(std::shared_ptr<Enemy> e)
{
    Point loc = e->getLocation();
    double rad = e->getRadius();
    RGBAcolor color = e->getBodyColor();
    double deltaRad = ENEMY_BLAST_RADIUS / EXPLOSION_LIFETIME;
    explosions.push_back(std::make_shared<Explosion>(loc, rad, EXPLOSION_LIFETIME, color, deltaRad, e->getNumber()));
}
void GameManager::createMissileExplosion(std::shared_ptr<Missile> m)
{
    Point loc = m->getLocation();
    double rad = m->getRadius();
    RGBAcolor color = m->getCoreColor();
    double deltaRad = MISSILE_BLAST_RADIUS / EXPLOSION_LIFETIME;
    explosions.push_back(std::make_shared<Explosion>(loc, rad, EXPLOSION_LIFETIME, color, deltaRad));
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
Point GameManager::getCameraLocation() const
{
    return player.getLocation();
}
Point GameManager::getCameraLookingAt() const
{
    return player.getLookingAt();
}
Point GameManager::getCameraUp() const
{
    return player.getUp();
}


// =============================
//
//        Game Management
//
// =============================

void GameManager::checkForGameEnd()
{
    if(playerScore > 999 && playerScore > computerScore)
    {
        gameResult = "You won " + std::to_string(playerScore) + " to " + std::to_string(computerScore);
        currentStatus = End;
    }
    else if(computerScore > 999 && computerScore > playerScore)
    {
        gameResult = "You lost " + std::to_string(computerScore) + " to " + std::to_string(playerScore);
        currentStatus = End;
    }
}

void GameManager::resetGame()
{
    playerScore = 0;
    computerScore = 0;
    enemies = std::vector<std::shared_ptr<Enemy>>();
    missiles = std::vector<std::shared_ptr<Missile>>();
    explosions = std::vector<std::shared_ptr<Explosion>>();
    computer = Computer({96, 12, 0}, 2, 0.1, &enemies, ENEMY_BLAST_RADIUS);
    player = Player();
    currentStatus = Playing;
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
    Point v = player.getLocation();
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

void GameManager::drawUI() const
{
    if(currentStatus == Intro)
    {
        playButton.draw();
        quitButton.draw();
    }
    else if(currentStatus == Playing)
    {
        drawCursor();
        drawPlayerDirection(screenWidth - screenHeight/10, 9*screenHeight/10);
        displayScores();
    }
    else if(currentStatus == End)
    {
        playAgainButton.draw();
        displayGameResult();
        quitButton.draw();
    }
}

void GameManager::drawCursor() const
{
    glBegin(GL_QUADS);
    glColor4f(0.8, 0, 0, cursorAlpha);
    glVertex3f(screenWidth/2 + 5,screenHeight/2 + 5,0);
    glVertex3f(screenWidth/2 - 5,screenHeight/2 + 5,0);
    glVertex3f(screenWidth/2 - 5,screenHeight/2 - 5,0);
    glVertex3f(screenWidth/2 + 5,screenHeight/2 - 5,0);
    glEnd();
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
    std::string score = "Player:   " + std::to_string(playerScore);
    glRasterPos2i(10 + (4 * score.length()), 25);
    for(const char &letter : score)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
    }

    score = "Computer: " + std::to_string(computerScore);
    glRasterPos2i(10 + (4 * score.length()), 45);
    for(const char &letter : score)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
    }
}

void GameManager::displayGameResult() const
{
    glRasterPos2i(screenWidth/2 - (4 * gameResult.length()), 3*screenHeight / 4);
    for(const char &letter : gameResult)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
    }
}