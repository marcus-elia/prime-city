#ifndef PRIME_CITY_MATHHELPER_H
#define PRIME_CITY_MATHHELPER_H

#include "structs.h"
#include <cmath>

// This file contains general math helper functions

// Directions
enum direction {Left, Right, Up, Down};

// Assuming n > 0, this returns the integer closest to n that
// is a perfect square
int nearestPerfectSquare(int n);

// Returns the square root of n, assuming that n is a perfect square
int isqrt(int n);

// Prime test, works for n = 0 through n = 120
bool twoDigitIsPrime(int n);

// Given the ID of a Chunk, this returns the Chunk coords
Point2D chunkIDtoPoint(int n);

// Return the unique id of the PlotNode, depending on which chunk it's in and
// where it is within the chunk
int makeID(Point2D chunkCoords, Point2D plotCoords, int plotsPerSide);
int makeID(int chunkID, Point2D plotCoords, int plotsPerSide);

Point2D idToPlotCoords(int id, int plotsPerSide);

// Get the id of the PlotNode adjacent to this one
int idAbove(int id, int plotsPerSide);
int idBelow(int id, int plotsPerSide);
int idLeft(int id, int plotsPerSide);
int idRight(int id, int plotsPerSide);

int getIDinDirection(int plotID, int plotsPerSide, direction dir);


// Returns the distance in the xz plane
double distance2d(Point p1, Point p2);

// Euclidean distance
double distance(Point2D p1, Point2D p2);

double distance3d(Point p1, Point p2);

// 2d directed distance
double directedDistance(double A, double B, double C, double x, double y);

// Returns the id of the plot whose center is closest to the given Point
// This should be the plot that the point is contained in
int getIDofNearestPlot(Point p, int chunkSize, int plotsPerSide);

// Given a plotID, return the center of the plot
Point getPlotCenterFromID(int plotID, int chunkSize, int plotsPerSide);

// Given a plotID, get the corners of the plot
Point getPlotTopLeftFromID(int plotID, int chunkSize, int plotsPerSide);
Point getPlotTopRightFromID(int plotID, int chunkSize, int plotsPerSide);
Point getPlotBottomLeftFromID(int plotID, int chunkSize, int plotsPerSide);
Point getPlotBottomRightFromID(int plotID, int chunkSize, int plotsPerSide);


// Compare points to lines in the xz-plane
// Returns true if the (x,z) part of the point is above the line,
// meaning more negative in the z-direction, than the line
bool pointAboveLine(Point p, double m, double b);

#endif //PRIME_CITY_MATHHELPER_H
