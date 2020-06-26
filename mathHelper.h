#ifndef PRIME_CITY_MATHHELPER_H
#define PRIME_CITY_MATHHELPER_H

#include "structs.h"
#include <cmath>

// This file contains general math helper functions

// Assuming n > 0, this returns the integer closest to n that
// is a perfect square
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

// Returns the square root of n, assuming that n is a perfect square
int sqrt(int n)
{
    return round(sqrt(n));
}

// Given the ID of a Chunk, this returns the Chunk coords
Point2D chunkIDtoPoint(int n)
{
    int s = nearestPerfectSquare(n);
    int sq = sqrt(s);
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
            return {-(sq + 1)/2 - s + n, (sq + 1)/2 - 1};
        }
    }
}


#endif //PRIME_CITY_MATHHELPER_H
