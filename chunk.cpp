#include "chunk.h"

Chunk::Chunk()
{
    bottomLeft = {0,0};
    sideLength = 1024;
    perlinSeed = 0.5;
    initializeCenter();
    plotSize = sideLength / plotsPerSide;
    initializePlotEmpty();
    makeBuildings();
}
Chunk::Chunk(Point2D inputBottomLeft, int inputSideLength,  double inputPerlinSeed)
{
    bottomLeft = inputBottomLeft;
    sideLength = inputSideLength;
    perlinSeed = inputPerlinSeed;
    initializeCenter();
    initializePlotEmpty();
    plotSize = sideLength / plotsPerSide;
    makeBuildings();
}

void Chunk::initializeCenter()
{
    center = {sideLength*bottomLeft.x + sideLength/2, sideLength*bottomLeft.z - sideLength/2};
}
void Chunk::initializePlotEmpty()
{
    for(int i = 0; i < plotsPerSide; i++)
    {
        for(int j = 0; j < plotsPerSide; j++)
        {
            plotEmpty[i][j] = true;
        }
    }
}
void Chunk::makeBuildings()
{
    srand(time(NULL));
    for(int i = 0; i < plotsPerSide; i++)
    {
        for(int j = 0; j < plotsPerSide; j++)
        {
            // Don't build around the origin where the player spawns
            if(abs(bottomLeft.x*sideLength + i*plotSize) < sideLength/2 &&
               abs((bottomLeft.z + 1)*sideLength - (j+1)*plotSize) < sideLength/2)
            {
                continue;
            }

            double r1 = (double)(rand() % 100) / 100;

            int colorSeed = rand() % 100;
            RGBAcolor color;
            if(colorSeed < 33)
            {
                color = {78/256.0, 65/256.0, 84/256.0, 1};  // Purple Silver
            }
            else if(colorSeed < 66)
            {
                color = {117/256.0, 87/256.0, 89/256.0, 1}; // Red Silver
            }
            else
            {
                color = {105/256.0, 90/256.0, 73/256.0, 1}; // Orange Silver
            }

            RGBAcolor edgeColor = {1.0, 1.0, 0.0, 1.0}; // Yellow

            double height;
            if(perlinSeed < 1.0)
            {
                height = 200 + r1*100 + perlinSeed*100 + rand() % 100;
            }
            else // If we are at a max height seeded plot, make buidlings taller at the center
            {
                Point2D currentPlot = {i, j};
                Point2D chunkCenter = {plotsPerSide/2,plotsPerSide/2};
                height = 250 + r1*100 + perlinSeed*100 + 25*(1.4*plotsPerSide/2 - distance(currentPlot, chunkCenter));
            }

            if((r1 < BUILDING_DENSITY || perlinSeed == 1.0) && plotEmpty[i][j] && shouldConsiderBuilding(i,j))
            {
                Point2D topLeftOfBuilding = {bottomLeft.x*sideLength + i*plotSize,
                                             (bottomLeft.z + 1)*sideLength - (j+1)*plotSize};
                buildings.push_back(Building(topLeftOfBuilding, plotSize, height,
                                             color, edgeColor, Plain));
                plotEmpty[i][j] = false;
            }
        }
    }
}

// Getters
Point2D Chunk::getBottomLeft() const
{
    return bottomLeft;
}
int Chunk::getSideLength() const
{
    return sideLength;
}
Point2D Chunk::getCenter() const
{
    return center;
}
int Chunk::getPlotsPerSide() const
{
    return plotsPerSide;
}
int Chunk::getPlotSize() const
{
    return plotSize;
}
std::vector<Building> Chunk::getBuildings() const
{
    return buildings;
}

bool Chunk::shouldConsiderBuilding(int i, int j) const
{
    int neighbors = 0;

    // Left
    if(i == 0)
    {
        if((rand() % 100) / 100.0 < BUILDING_DENSITY)
        {
            neighbors++;
        }

        // Up Left
        if((rand() % 100) / 100.0 < BUILDING_DENSITY)
        {
            neighbors++;
        }
        // Down left
        if((rand() % 100) / 100.0 < BUILDING_DENSITY)
        {
            neighbors++;
        }
    }
    else
    {
        if(!plotEmpty[i-1][j])
        {
            neighbors++;
        }

        // Up left
        if(j == 0)
        {
            if((rand() % 100) / 100.0 < BUILDING_DENSITY)
            {
                neighbors++;
            }
        }
        else
        {
            if(!plotEmpty[i-1][j-1])
            {
                neighbors++;
            }
        }
        // Down left
        if(j == plotsPerSide-1)
        {
            if((rand() % 100) / 100.0 < BUILDING_DENSITY)
            {
                neighbors++;
            }
        }
        else
        {
            if(!plotEmpty[i-1][j+1])
            {
                neighbors++;
            }
        }
    }

    // Right
    if(i == plotsPerSide-1)
    {
        if((rand() % 100) / 100.0 < BUILDING_DENSITY)
        {
            neighbors++;
        }

        // Up Right
        if((rand() % 100) / 100.0 < BUILDING_DENSITY)
        {
            neighbors++;
        }
        // Down Right
        if((rand() % 100) / 100.0 < BUILDING_DENSITY)
        {
            neighbors++;
        }
    }
    else
    {
        if(!plotEmpty[i+1][j])
        {
            neighbors++;
        }

        // Up Right
        if(j == 0)
        {
            if((rand() % 100) / 100.0 < BUILDING_DENSITY)
            {
                neighbors++;
            }
        }
        else
        {
            if(!plotEmpty[i+1][j-1])
            {
                neighbors++;
            }
        }
        // Down right
        if(j == plotsPerSide-1)
        {
            if((rand() % 100) / 100.0 < BUILDING_DENSITY)
            {
                neighbors++;
            }
        }
        else
        {
            if(!plotEmpty[i+1][j+1])
            {
                neighbors++;
            }
        }
    }

    // Up
    if(j == 0)
    {
        if((rand() % 100) / 100.0 < BUILDING_DENSITY)
        {
            neighbors++;
        }
    }
    else
    {
        if(!plotEmpty[i][j-1])
        {
            neighbors++;
        }
    }

    // Down
    if(j == plotsPerSide-1)
    {
        if((rand() % 100) / 100.0 < BUILDING_DENSITY)
        {
            neighbors++;
        }
    }
    else
    {
        if(!plotEmpty[i][j+1])
        {
            neighbors++;
        }
    }
    return neighbors < BUILDING_DENSITY*8;
}



void Chunk::draw() const
{
    glBegin(GL_QUADS);
    /*if((bottomLeft.x + bottomLeft.z) % 2 == 0)
    {
        glColor4f(1, 1, 0.3, 1);
    }
    else
    {
        glColor4f(0, 1, 0.8, 1);
    }*/
    //glColor4f(perlinSeed, 0, 1, 1);
    glColor4f(0.0, 0.2, 0.45, 1); // Navy blue
    glVertex3f(sideLength*bottomLeft.x,0, sideLength*bottomLeft.z);
    glVertex3f(sideLength*bottomLeft.x,0, sideLength*bottomLeft.z + sideLength);
    glVertex3f(sideLength*bottomLeft.x + sideLength,0, sideLength*bottomLeft.z + sideLength);
    glVertex3f(sideLength*bottomLeft.x + sideLength,0, sideLength*bottomLeft.z);

    glEnd();

    for(const Building &b : buildings)
    {
        b.draw();
    }
}

int Chunk::chunkToInt() const
{
    return pointToInt({bottomLeft.x, bottomLeft.z});
}

std::vector<Point2D> Chunk::getChunksAround(int radius)
{
    return getChunksAroundPointByPoint(bottomLeft, radius);
}

int pointToInt(Point2D p)
{
    return p.toChunkID();
}

std::vector<int> getChunksAroundPoint(Point2D p, int radius)
{
    std::vector<int> result;

    // Start at the top of the diamond and work down from there
    for(int b = p.z + radius; b >= p.z - radius; b--)
    {
        int distanceFromZ = abs(b - p.z);
        for(int a = p.x - (radius - distanceFromZ); a <= p.x + (radius - distanceFromZ); a++)
        {
            result.push_back(pointToInt({a,b}));
        }
    }
    return result;
}
std::vector<Point2D> getChunksAroundPointByPoint(Point2D p, int radius)
{
    std::vector<Point2D> result;

    // Start at the top of the diamond and work down from there
    for(int b = p.z + radius; b >= p.z - radius; b--)
    {
        int distanceFromZ = abs(b - p.z);
        for(int a = p.x - (radius - distanceFromZ); a <= p.x + (radius - distanceFromZ); a++)
        {
            result.push_back({a,b});
        }
    }
    return result;
}

std::experimental::optional<Point> Chunk::correctCollision(Point p, int buffer)
{
    for(Building& b : buildings)
    {
        std::experimental::optional<Point> newPoint = b.correctCollision(p, buffer);
        if(newPoint)
        {
            return newPoint;
        }
    }
    return std::experimental::nullopt;
}