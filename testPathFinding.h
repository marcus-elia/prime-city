#ifndef PRIME_CITY_TESTPATHFINDING_H
#define PRIME_CITY_TESTPATHFINDING_H

// This file is used to make functions to test the PlotNetwork and breadth
// first search path finding because testing.cpp is getting too long.

#include "plotNetwork.h"
#include <iostream>

/*
 * + - - - - - - - - +
 * | X s o o X o o o |
 * | o o X o o o X X |
 * | o X X X X o X X |
 * | o o o X X o o o |
 * | o X X X X X X o |
 * | o o X X X o o o |
 * | X o X X X o X X |
 * | X o o o o o o F |
 * + - - - - - - - - +
 */
// Straightforward finding the shortest path
bool testBFSexample1();

/*
 * + - - - - - - - - + - - - - - - - - +
 * | X s o o X o o o | o o o o o o X F |
 * | o o X o o o X X | o X X X X o o o |
 * | o X X X X o X X | o X X X X X X X |
 * | o o o X X o o o | o X X X X X X X |
 * | o X X X X X X o | o X X X X X X X |
 * | o o X X X o o o | o X X X X X X X |
 * | X o X X X o X X | o X X X X X X X |
 * | X o o o o o o o | o X X X X X X X |
 * + - - - - - - - - + - - - - - - - - +
 */
// Going into a new chunk to find it
bool testBFSexample2();


/*
 * + - - - - - - - - +
 * | X s o o X X X X |
 * | o o X o X X X X |
 * | o X X X X X X X |
 * | o o o X X X X X |
 * | o X X X X X X X |
 * | o o X X X X X X |
 * | X o X X X X X X |
 * | X o o o o X o F |
 * + - - - - - - - - +
 */
// When it is impossible to reach
bool testBFSexample3();

/*
 * + - - - - - - - - +
 * | X s o o X X X X |
 * | o o X o o X X X |
 * | o X X X o X X X |
 * | o o o X o X X X |
 * | o X X X X X X X |
 * | o o X X X X X X |
 * | X o X X X X X X |
 * | X o o o o X o F |
 * + - - - - - - - - +
 */
// When path length is limited
bool testBFSexample4();


bool testGetPlotIDsBetween();




/*
 * + - - - - - - - - +
 * | X s o o X X X X |
 * | X o o o o X X X |
 * | X X o o o X X X |
 * | X X X X o X X X |
 * | X X X X o X X X |
 * | X X X X F X X X |
 * | X X X X X X X X |
 * | X X X X X X X X |
 * + - - - - - - - - +
 */
bool testClippingExample1();

// Just a straight line
bool testClippingExample2();

/*
 * + - - - - - - - - +
 * | X s o o o X X X |
 * | X o o o o X X X |
 * | X X o o o X X X |
 * | X X X X o X X X |
 * | X X X X o X X X |
 * | X o o o o X X X |
 * | o o o X X X X X |
 * | F o X X X X X X |
 * + - - - - - - - - +
 */
bool testClippingExample3();

/*
 * + - - - - - - - - +
 * | X X X X X X X X |
 * | X X X X X X X X |
 * | X X X X X X X X |
 * | X X X X X X X X |
 * | X X X X X X X F |
 * | X X X X X X o o |
 * | X X X X X o o X |
 * | X X X X s o X X |
 * + - - - - - - - - +
 */
bool testClippingExample4();

/*
 * + - - - - - - - - + - - - - - - - - +
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X F o | B s X X X X X X |
 * | X X X X X X o o | o o X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X x X X X | X X X X X X X X |
 * + - - - - - - - - + - - - - - - - - +
 */
bool testClippingExample5();

/*
 * + - - - - - - - - +
 * | X o o o o X X X |
 * | X o o o o X X X |
 * | X X o o o X X X |
 * | X o X X o X o o |
 * | X X X X o o o X |
 * | X o o o o X X o |
 * | o o o X X X X X |
 * | o o X X X X X X |
 * + - - - - - - - - +
 */
bool testHasLineOfSight();

#endif //PRIME_CITY_TESTPATHFINDING_H
