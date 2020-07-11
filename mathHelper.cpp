#include "mathHelper.h"

int nearestPerfectSquare(int n)
{
    int squareJumpAmount = 3;
    int curSquare = 1;
    int prevSquare = 0;
    while(curSquare < n)
    {
        prevSquare = curSquare;
        curSquare += squareJumpAmount;
        squareJumpAmount += 2;  // the difference between consecutive squares is odd integer
    }
    if(n - prevSquare > curSquare - n)
    {
        return curSquare;
    }
    else
    {
        return prevSquare;
    }
}

int isqrt(int n)
{
    return round(sqrt(n));
}

bool twoDigitIsPrime(int n)
{
    if(n < 2)
    {
        return false;
    }
    return n % 2 != 0 && n % 3 != 0 && n % 5 != 0 && n % 7 != 0;
}

Point2D chunkIDtoPoint(int n)
{
    int s = nearestPerfectSquare(n);
    int sq = isqrt(s);
    if(s % 2 == 0)
    {
        if(n >= s)
        {
            return {sq/2, -sq/2 + n - s};
        }
        else
        {
            return {sq/2 - s + n, -sq/2};
        }
    }
    else
    {
        if(n >= s)
        {
            return {-(sq + 1)/2, (sq + 1)/2 - 1 - n + s};
        }
        else
        {
            return {-(sq + 1)/2 + s - n, (sq + 1)/2 - 1};
        }
    }
}

int makeID(Point2D chunkCoords, Point2D plotCoords, int plotsPerSide)
{
    return chunkCoords.toChunkID()*plotsPerSide*plotsPerSide + plotCoords.x + plotCoords.z * plotsPerSide;
}
int makeID(int chunkID, Point2D plotCoords, int plotsPerSide)
{
    return chunkID*plotsPerSide*plotsPerSide + plotCoords.x + plotCoords.z * plotsPerSide;
}

Point2D idToPlotCoords(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    id = id % plotsPerChunk;
    int x = id % plotsPerSide;
    int z = id / plotsPerSide;
    return {x, z};
}

int idAbove(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    Point2D plotCoords = idToPlotCoords(id, plotsPerSide);
    int chunkID = id / plotsPerChunk;
    int x = plotCoords.x;
    int z = plotCoords.z;
    if(z > 0) // If it's in the same chunk
    {
        return makeID(chunkID, {x, z-1}, plotsPerSide);
    }
    else     // If it's in the chunk above this one
    {
        Point2D chunkCoords = chunkIDtoPoint(chunkID);
        chunkCoords.z -= 1; // move up one chunk
        return makeID(chunkCoords, {x, plotsPerSide-1}, plotsPerSide);
    }
}
int idBelow(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    Point2D plotCoords = idToPlotCoords(id, plotsPerSide);
    int chunkID = id / plotsPerChunk;
    int x = plotCoords.x;
    int z = plotCoords.z;
    if(z < plotsPerSide - 1) // If it's in the same chunk
    {
        return makeID(chunkID, {x, z+1}, plotsPerSide);
    }
    else     // If it's in the chunk above this one
    {
        Point2D chunkCoords = chunkIDtoPoint(chunkID);
        chunkCoords.z += 1; // move down one chunk
        return makeID(chunkCoords, {x, 0}, plotsPerSide);
    }
}
int idLeft(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    Point2D plotCoords = idToPlotCoords(id, plotsPerSide);
    int chunkID = id / plotsPerChunk;
    int x = plotCoords.x;
    int z = plotCoords.z;
    if(x > 0) // If it's in the same chunk
    {
        return makeID(chunkID, {x-1, z}, plotsPerSide);
    }
    else     // If it's in the chunk above this one
    {
        Point2D chunkCoords = chunkIDtoPoint(chunkID);
        chunkCoords.x -= 1; // move left one chunk
        return makeID(chunkCoords, {plotsPerSide-1, z}, plotsPerSide);
    }
}
int idRight(int id, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    Point2D plotCoords = idToPlotCoords(id, plotsPerSide);
    int chunkID = id / plotsPerChunk;
    int x = plotCoords.x;
    int z = plotCoords.z;
    if(x < plotsPerSide-1) // If it's in the same chunk
    {
        return makeID(chunkID, {x+1, z}, plotsPerSide);
    }
    else     // If it's in the chunk above this one
    {
        Point2D chunkCoords = chunkIDtoPoint(chunkID);
        chunkCoords.x += 1; // move right one chunk
        return makeID(chunkCoords, {0, z}, plotsPerSide);
    }
}

int getIDinDirection(int plotID, int plotsPerSide, direction dir)
{
    if(dir == Left)
    {
        return idLeft(plotID, plotsPerSide);
    }
    else if(dir == Right)
    {
        return idRight(plotID, plotsPerSide);
    }
    else if(dir == Up)
    {
        return idAbove(plotID, plotsPerSide);
    }
    else
    {
        return idBelow(plotID, plotsPerSide);
    }
}

double distance2d(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.z - p2.z)*(p1.z - p2.z));
}

double distance(Point2D p1, Point2D p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.z - p2.z)*(p1.z - p2.z));
}

double distance3d(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) +
                (p1.y - p2.y)*(p1.y - p2.y) +
                (p1.z - p2.z)*(p1.z - p2.z));
}

double directedDistance(double A, double B, double C, double x, double y)
{
    return (A*x + B*y + C) / sqrt(A*A + B*B);
}

int getIDofNearestPlot(Point p, int chunkSize, int plotsPerSide)
{
    Point2D chunkCoords = {(int)floor(p.x / chunkSize), (int)floor(p.z / chunkSize)};
    int plotSize = chunkSize / plotsPerSide;
    int plotX = (int)floor((p.x - chunkCoords.x*chunkSize) / plotSize);
    int plotZ = (int)floor((p.z - chunkCoords.z*chunkSize) / plotSize);
    Point2D plotCoords = {plotX, plotZ};
    return makeID(chunkCoords, plotCoords, plotsPerSide);
}

Point getPlotCenterFromID(int plotID, int chunkSize, int plotsPerSide)
{
    int plotsPerChunk = plotsPerSide*plotsPerSide;
    int chunkID = plotID / plotsPerChunk;
    Point2D chunkCoords = chunkIDtoPoint(chunkID);
    Point2D plotCoords = idToPlotCoords(plotID, plotsPerSide);
    int plotSize = chunkSize / plotsPerSide;
    double x = chunkCoords.x*chunkSize + plotCoords.x*plotSize;
    double z = chunkCoords.z*chunkSize + plotCoords.z*plotSize;
    return {x, 0, z};
}
Point getPlotTopLeftFromID(int plotID, int chunkSize, int plotsPerSide)
{
    Point p = getPlotCenterFromID(plotID, chunkSize, plotsPerSide);
    int plotSizeOver2 = chunkSize / plotsPerSide / 2;
    return {p.x - plotSizeOver2, 0, p.z - plotSizeOver2};
}
Point getPlotTopRightFromID(int plotID, int chunkSize, int plotsPerSide)
{
    Point p = getPlotCenterFromID(plotID, chunkSize, plotsPerSide);
    int plotSizeOver2 = chunkSize / plotsPerSide / 2;
    return {p.x + plotSizeOver2, 0, p.z - plotSizeOver2};
}
Point getPlotBottomLeftFromID(int plotID, int chunkSize, int plotsPerSide)
{
    Point p = getPlotCenterFromID(plotID, chunkSize, plotsPerSide);
    int plotSizeOver2 = chunkSize / plotsPerSide / 2;
    return {p.x - plotSizeOver2, 0, p.z + plotSizeOver2};
}
Point getPlotBottomRightFromID(int plotID, int chunkSize, int plotsPerSide)
{
    Point p = getPlotCenterFromID(plotID, chunkSize, plotsPerSide);
    int plotSizeOver2 = chunkSize / plotsPerSide / 2;
    return {p.x + plotSizeOver2, 0, p.z + plotSizeOver2};
}

bool pointAboveLine(Point p, double m, double b)
{
    double pluggedInZ = m*p.x + b;
    return p.z < pluggedInZ;
}