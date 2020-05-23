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
    /*srand(time(NULL));
    for(int i = 0; i < sideLength / propertySize; i++)
    {
        for(int j = 0; j < sideLength / propertySize; j++)
        {
            // If the building is too close to the origin, don't build it
            if(abs(bottomLeft.x*sideLength + i*propertySize) < sideLength/3 &&
               abs((bottomLeft.z + 1)*sideLength - (j+1)*propertySize) < sideLength/3)
            {
                continue;
            }

            double r1 = (double)(rand() % 100) / 100;
            double r2 = (double)(rand() % 100) / 100;

            // Make a building if r says yes, more likely when perlin noise is high
            if(r1 < perlinSeed && r2 > 0.8)
            {
                Point2D topLeftOfBuilding = {bottomLeft.x*sideLength + i*propertySize,
                                   (bottomLeft.z + 1)*sideLength - (j+1)*propertySize};
                // What type of building is it? How high is it?
                typeOfBuilding buildingType;
                int height;
                if(perlinSeed < 0.25 || (perlinSeed < 0.5 && r1 < 0.1))
                {
                    buildingType = House;
                    height = propertySize/2;
                }
                else if(r1 > 0.85 && perlinSeed > 0.75)
                {
                    buildingType = Skyscraper;
                    height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                }
                else if(0.55 < r1 && r1 < 0.6 && perlinSeed > 0.4)
                {
                    buildingType = Pyramid;
                    height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                }
                else if(0.7 < r1 && r1 < 0.75 && perlinSeed > 0.65)
                {
                    buildingType = Hourglass;
                    height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                }
                else if(0.1 < r1  && r1 < 0.15 && perlinSeed > 0.65)
                {
                    buildingType = Empire;
                    height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                }
                else if(0.45 < r1 && r1 < 0.5 && perlinSeed > 0.65)
                {
                    buildingType = UFO;
                    height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                }
                else if(perlinSeed > 0.95 && r1 < 0.05 && r2 > 0.9)
                {
                    buildingType = CN;
                    height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                }
                else
                {
                    buildingType = Plain;
                    height =  (int)(perlinSeed*180 + r1*80 + r2*80);
                }
                buildings.push_back(Building(topLeftOfBuilding, propertySize, height,
                        {r1, 0, perlinSeed, 1}, {1,1,1,1}, buildingType));
            }
        }
    }*/
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
/*std::vector<Building> Chunk::getBuildings() const
{
    return buildings;
}*/

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
    glColor4f(0.5, 0.5, 0.5, 1);
    glVertex3f(sideLength*bottomLeft.x,0, sideLength*bottomLeft.z);
    glVertex3f(sideLength*bottomLeft.x,0, sideLength*bottomLeft.z + sideLength);
    glVertex3f(sideLength*bottomLeft.x + sideLength,0, sideLength*bottomLeft.z + sideLength);
    glVertex3f(sideLength*bottomLeft.x + sideLength,0, sideLength*bottomLeft.z);

    glEnd();

    /*for(const Building &b : buildings)
    {
        b.draw();
    }*/
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
    int a = p.x;
    int b = p.z;
    if(a > 0 && b <= a-1 && b > -a)
    {
        return 4*a*a - 3*a - b;
    }
    else if(b < 0 && a <= -b && a > b+1)
    {
        return 4*b*b + b - a;
    }
    else if(a < 0 && b > a-1 && b < -a)
    {
        return 4*a*a - a + b;
    }
    else if(b > -1 && a >= -b && a < b+1)
    {
        return 4*b*b + 3*b + a;
    }
    return (2*a-1)*(2*a-3); // idk why, but this works for (0,-1) and (-1,-2)
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
    /*for(Building& b : buildings)
    {
        std::experimental::optional<Point> newPoint = b.correctCollision(p, buffer);
        if(newPoint)
        {
            return newPoint;
        }
    }*/
    return std::experimental::nullopt;
}