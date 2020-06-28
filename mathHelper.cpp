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
    return chunkCoords.toChunkID() + plotCoords.x + plotCoords.z * plotsPerSide;
}
int makeID(int chunkID, Point2D plotCoords, int plotsPerSide)
{
    return chunkID + plotCoords.x + plotCoords.z * plotsPerSide;
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
