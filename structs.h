#ifndef PRIME_CITY_STRUCTS_H
#define PRIME_CITY_STRUCTS_H

// This file contains some structs and enums to make
// including simpler


// int points used for the chunks
// in the xz plane
struct Point2D
{
    int x;
    int z;
    bool operator ==(const Point2D& p1)
    {
        return x == p1.x && z == p1.z;
    }
    bool operator !=(const Point2D& p1)
    {
        return x != p1.x || z != p1.z;
    }
    int toChunkID()
    {
        int a = x;
        int b = z;
        // Bottom Zone
        if(b > 0 && a >= -b && a < b)
        {
            return 4*b*b + 3*b - a;
        }
        // Left Zone
        else if(a < 0 && b < -a && b >= a)
        {
            return 4*a*a + 3*a - b;
        }
        // Top Zone
        else if(b < 0 && a <= -b && a > b)
        {
            return 4*b*b + b + a;
        }
        // Right Zone
        else if(a > 0 && b <= a && b > -a)
        {
            return 4*a*a + a + b;
        }
        // Only a=0, b=0 is not in a zone
        else
        {
            return 0;
        }


    }
};


struct Point
{
    double x;
    double y;
    double z;

    // m and b represent a line such that z = mx + b.
    // This determines if p lies above the line in the xz plane
    bool isAboveLine(double m, double b)
    {
        return z < m*x + b; // Reverse inequality because up is negative z
    }
};

// A line segment specified by two points
struct Segment
{
    Point p1;
    Point p2;
};

struct RGBAcolor
{
    double r;
    double g;
    double b;
    double a;
};




#endif //PRIME_CITY_STRUCTS_H
