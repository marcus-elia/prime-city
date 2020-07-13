#include "recPrism.h"
#include "cylinder.h"
#include "frustum.h"
#include "chunk.h"
#include <experimental/optional>
#include "mathHelper.h"
#include "testPathFinding.h"

bool testIsAboveLine();

bool testCorrectRectangularCrossSection();

bool testCorrectRectangularPrismCollision();

bool testCorrectEllipticalCrossSection();

bool testPointToIntAndChunkIDtoPoint();

bool testNearestPerfectSquare();

bool testiSqrt();

bool testMakeID();

bool testIdToPlotCoords();

bool testIDAboveBelowLeftRight();

bool testGetIDofNearestPlot();


int main()
{
    testIsAboveLine();
    testCorrectRectangularCrossSection();
    testCorrectRectangularPrismCollision();
    testCorrectEllipticalCrossSection();
    testPointToIntAndChunkIDtoPoint();
    testNearestPerfectSquare();
    testiSqrt();
    testMakeID();
    testIdToPlotCoords();
    testIDAboveBelowLeftRight();
    testBFSexample1();
    testBFSexample2();
    testBFSexample3();
    testBFSexample4();
    testGetPlotIDsBetween();
    testClippingExample1();
    testClippingExample2();
    testClippingExample3();
    testClippingExample4();
    return 0;
}


bool testIsAboveLine()
{
    bool passed = true;
    std::cout << "\nTesting isAboveLine()" << std::endl;

    Point p;
    double m, b;

    m = 1;
    b = 0;
    // Positive slope through origin, point above line
    p = {0, 0, -3};
    if(!p.isAboveLine(m, b))
    {
        passed = false;
        std::cout << "FAILED test of pos slope through origin, point above line." << std::endl;
    }
    // Positive slope through origin, point on line
    p = {5, 0, 5};
    if(p.isAboveLine(m, b))
    {
        passed = false;
        std::cout << "FAILED test of pos slope through origin, point on line." << std::endl;
    }
    // Positive slope through origin, point under line
    p = {0, 0, 5};
    if(p.isAboveLine(m, b))
    {
        passed = false;
        std::cout << "FAILED test of pos slope through origin, point under line." << std::endl;
    }

    m = -1;
    b = 0;
    // Negative slope through origin, point above line
    p = {0, 0, -3};
    if(!p.isAboveLine(m, b))
    {
        passed = false;
        std::cout << "FAILED test of neg slope through origin, point above line." << std::endl;
    }
    // Negative slope through origin, point on line
    p = {3, 0, -3};
    if(p.isAboveLine(m, b))
    {
        passed = false;
        std::cout << "FAILED test of neg slope through origin, point on line." << std::endl;
    }
    // Negative slope through origin, point below line
    p = {0, 0, 3};
    if(p.isAboveLine(m, b))
    {
        passed = false;
        std::cout << "FAILED test of neg slope through origin, point below line." << std::endl;
    }

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}


/*
 * + - - - - - +
 * |  \  1  /  |
 * | 4   X   2 |
 * |  /  3  \  |
 * + - - - - - +
 */
bool testCorrectRectangularCrossSection()
{
    bool passed = true;
    std::cout << "\nTesting correctRectangularCrossSection()" << std::endl;

    const double TOLERANCE = 0.01;
    Point p, c;
    double xw, zw;
    int buffer;
    std::experimental::optional<Point> obs;
    Point exp;

    // When the Point does not need to be corrected
    c = {0, 0, 0};
    xw = 10;
    zw = 20;
    buffer = 2;

    // Way too far left
    p = {-30, 0, 0};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point way too far left." << std::endl;
    }
    // On the left buffer
    p = {-7, 0, 0};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point on left buffer." << std::endl;
    }
    // Way too far right
    p = {30, 0, 0};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point way too far right." << std::endl;
    }
    // On the right buffer
    p = {7, 0, 0};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point on right buffer." << std::endl;
    }
    // Way too far up
    p = {0, 0, -30};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point way too far up." << std::endl;
    }
    // On the top buffer
    p = {0, 0, -12};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point on top buffer." << std::endl;
    }
    // Way too far down
    p = {0, 0, 30};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point way too far down." << std::endl;
    }
    // On the bottom buffer
    p = {0, 0, 12};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point on bottom buffer." << std::endl;
    }

    // Inside zone 1
    p = {0, 0, -2};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    exp = {0, 0, -12};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED test of point inside zone 1." << std::endl;
    }
    // Inside zone 2
    p = {2, 0, 0};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    exp = {7, 0, 0};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED test of point inside zone 2." << std::endl;
    }
    // Inside zone 3
    p = {0, 0, 2};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    exp = {0, 0, 12};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED test of point inside zone 3." << std::endl;
    }
    // Inside zone 4
    p = {-2, 0, 0};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    exp = {-7, 0, 0};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED test of point inside zone 4." << std::endl;
    }

    // Point slightly N of NW and outside
    p = {-7, 0, -13};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point slightly N of NW and outside." << std::endl;
    }
    // Point slightly W of NW and outside
    p = {-8, 0, -12};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point slightly W of NW and outside." << std::endl;
    }
    // Point slightly N of NE and outside
    p = {7, 0, -13};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point slightly N of NE and outside." << std::endl;
    }
    // Point slightly E of NE and outside
    p = {8, 0, -12};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point slightly E of NE and outside." << std::endl;
    }
    // Point slightly S of SE and outside
    p = {7, 0, 13};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point slightly S of SE and outside." << std::endl;
    }
    // Point slightly E of SE and outside
    p = {8, 0, 12};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point slightly E of SE and outside." << std::endl;
    }
    // Point slightly S of SW and outside
    p = {-7, 0, 13};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point slightly S of SW and outside." << std::endl;
    }
    // Point slightly W of SW and outside
    p = {-8, 0, 12};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point slightly W of SW and outside." << std::endl;
    }


    // Point slightly N of NW and inside buffer
    p = {-5.2, 0, -10.5};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    exp = {-5.2, 0, -12};
    if(!obs || distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED test of point slightly N of NW and inside buffer." << std::endl;
    }

    // Point 1 from real life failed example
    p = {0, 5, 0};
    buffer = 5;
    c = {32, 54.5, 224};
    xw = 64;
    zw = 64;
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of real life example 1." << std::endl;
    }

    // Point 2 from real life failed example
    p = {1.176, 5, -3.823};
    buffer = 5;
    c = {96, 110, -352};
    xw = 64;
    zw = 64;
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of real life example 2." << std::endl;
    }


    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testCorrectRectangularPrismCollision()
{
    bool passed = true;
    std::cout << "\nTesting correctRectangularPrismCollision()" << std::endl;

    Point p, c;
    double xw, yw, zw;
    int buffer = 2;
    const double TOLERANCE = 0.01;

    c = {0,0,0};
    xw = 10;
    yw = 40;
    zw = 20;
    std::experimental::optional<Point> obs;
    Point exp;

    // Point way too high
    p = {0, 50, 0};
    obs = correctRectangularPrismCollision(p, buffer, c, xw, yw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point way too high." << std::endl;
    }
    // Point on top buffer
    p = {0, 22, 0};
    obs = correctRectangularPrismCollision(p, buffer, c, xw, yw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point on top buffer." << std::endl;
    }
    // Point way too low
    p = {0, -50, 0};
    obs = correctRectangularPrismCollision(p, buffer, c, xw, yw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point way too low." << std::endl;
    }
    // Point on bottom buffer
    p = {0, -22, 0};
    obs = correctRectangularPrismCollision(p, buffer, c, xw, yw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point on bottom buffer." << std::endl;
    }

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testCorrectEllipticalCrossSection()
{
    bool passed = true;
    std::cout << "\nTesting correctEllipticalCrossSection()" << std::endl;

    Point p, c;
    double xw, zw;
    int buffer = 2;
    const double TOLERANCE = 0.01;
    std::experimental::optional<Point> obs;
    Point exp;

    // x-axis is major
    c = {0, 0, 0};
    xw = 20, zw = 10;

    // =========================
    // Point far outside ellipse
    // =========================
    // Right
    p = {16, 0, 0};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED case of point far right of ellipse." << std::endl;
    }
    // Up Right
    p = {16, 0, -16};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED case of point far up right of ellipse." << std::endl;
    }
    // Up
    p = {0, 0, -16};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED case of point far up of ellipse." << std::endl;
    }
    // Up Left
    p = {-16, 0, -16};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED case of point far up left of ellipse." << std::endl;
    }
    // Left
    p = {-16, 0, 0};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED case of point far left of ellipse." << std::endl;
    }
    // Down Left
    p = {-16, 0, 16};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED case of point far down left of ellipse." << std::endl;
    }
    // Down
    p = {0, 0, 16};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED case of point far down of ellipse." << std::endl;
    }
    // Down Right
    p = {16, 0, 16};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED case of point far down right of ellipse." << std::endl;
    }

    // =========================
    //   Point inside ellipse
    // =========================
    // Right
    p = {8, 0, 0};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    exp = {12, 0, 0};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED case of point in ellipse, right." << std::endl;
    }

    // Up Right
    p = {4, 0, -2};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    exp = {7.965495, 0, -5.324388};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED case of point in ellipse, up right." << std::endl;
    }

    // Up
    p = {0, 0, -2};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    exp = {0, 0, -7};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED case of point in ellipse, up." << std::endl;
    }

    // Up Left
    p = {-4, 0, -2};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    exp = {-7.965495, 0, -5.324388};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED case of point in ellipse, up left." << std::endl;
    }

    // Left
    p = {-4, 0, 0};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    exp = {-12, 0, 0};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED case of point in ellipse, left." << std::endl;
    }

    // Down Left
    p = {-4, 0, 2};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    exp = {-7.965495, 0, 5.324388};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED case of point in ellipse, down left." << std::endl;
        std::cout << obs.value().x << std::endl;
        std::cout << obs.value().z << std::endl;
    }

    // Down
    p = {0, 0, 2};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    exp = {0, 0, 7};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED case of point in ellipse, down." << std::endl;
    }

    // Down Right
    p = {4, 0, 2};
    obs = correctEllipticalCrossSection(p, buffer, c, xw, zw);
    exp = {7.965495, 0, 5.324388};
    if(distance2d(exp, obs.value()) > TOLERANCE)
    {
        passed = false;
        std::cout << "FAILED case of point in ellipse, down right." << std::endl;
    }




    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testPointToIntAndChunkIDtoPoint()
{
    std::cout << std::endl << "Testing PointToInt()" << std::endl;

    bool passed = true;
    std::vector<Point2D> values;
    std::vector<int> expected;

    // Make a bunch of tests
    values.push_back({0,0});
    expected.push_back(0);
    values.push_back({-1,0});
    expected.push_back(1);
    values.push_back({-1,-1});
    expected.push_back(2);
    values.push_back({0,-1});
    expected.push_back(3);
    values.push_back({1,-1});
    expected.push_back(4);
    values.push_back({1,0});
    expected.push_back(5);
    values.push_back({1,1});
    expected.push_back(6);
    values.push_back({0,1});
    expected.push_back(7);
    values.push_back({-1,1});
    expected.push_back(8);
    values.push_back({-2,1});
    expected.push_back(9);
    values.push_back({-2,0});
    expected.push_back(10);
    values.push_back({-2,-1});
    expected.push_back(11);
    values.push_back({-2,-2});
    expected.push_back(12);
    values.push_back({-1,-2});
    expected.push_back(13);
    values.push_back({0,-2});
    expected.push_back(14);
    values.push_back({1,-2});
    expected.push_back(15);
    values.push_back({2,-2});
    expected.push_back(16);
    values.push_back({2,-1});
    expected.push_back(17);
    values.push_back({2,0});
    expected.push_back(18);
    values.push_back({2,1});
    expected.push_back(19);
    values.push_back({2,2});
    expected.push_back(20);
    values.push_back({1,2});
    expected.push_back(21);
    values.push_back({0,2});
    expected.push_back(22);
    values.push_back({2,-3});
    expected.push_back(35);
    values.push_back({-4,3});
    expected.push_back(49);

    for(int i = 0; i < values.size(); i++)
    {
        int observed = values[i].toChunkID();
        if(observed != expected[i])
        {
            passed = false;
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected " << expected[i] << ", observed " << observed << std::endl;
        }
    }

    std::cout << std::endl << "Testing chunkIDtoPoint()" << std::endl;
    for(int i = 0; i < values.size(); i++)
    {
        Point2D observed = chunkIDtoPoint(expected[i]);
        if(observed != values[i])
        {
            passed = false;
            std::cout << "Test FAILED when n = " << expected[i] << std::endl;
        }
    }

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testNearestPerfectSquare()
{
    bool passed = true;
    std::cout << "\nTesting nearestPerfectSquare()" << std::endl;

    std::vector<int> values;
    std::vector<int> expected;

    values.push_back(1);
    expected.push_back(1);
    values.push_back(2);
    expected.push_back(1);
    values.push_back(3);
    expected.push_back(4);
    values.push_back(4);
    expected.push_back(4);
    values.push_back(5);
    expected.push_back(4);
    values.push_back(6);
    expected.push_back(4);
    values.push_back(7);
    expected.push_back(9);
    values.push_back(8);
    expected.push_back(9);
    values.push_back(9);
    expected.push_back(9);
    values.push_back(10);
    expected.push_back(9);
    values.push_back(11);
    expected.push_back(9);
    values.push_back(12);
    expected.push_back(9);
    values.push_back(13);
    expected.push_back(16);
    values.push_back(65535);
    expected.push_back(65536);
    values.push_back(65537);
    expected.push_back(65536);

    for(int i = 0; i < values.size(); i++)
    {
        int observed = nearestPerfectSquare(values[i]);
        if(observed != expected[i])
        {
            passed = false;
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected " << expected[i] << ", observed " << observed << std::endl;
        }
    }
    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testiSqrt()
{
    bool passed = true;
    std::cout << "\nTesting isqrt()" << std::endl;

    std::vector<int> values;
    std::vector<int> expected;

    values.push_back(1);
    expected.push_back(1);
    values.push_back(4);
    expected.push_back(2);
    values.push_back(9);
    expected.push_back(3);
    values.push_back(16);
    expected.push_back(4);
    values.push_back(361);
    expected.push_back(19);
    values.push_back(65536);
    expected.push_back(256);

    for(int i = 0; i < values.size(); i++)
    {
        int observed = isqrt(values[i]);
        if(observed != expected[i])
        {
            passed = false;
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected " << expected[i] << ", observed " << observed << std::endl;
        }
    }
    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testMakeID()
{
    bool passed = true;
    std::cout << "\nTesting makeID()" << std::endl;

    Point2D chunkCoords;
    Point2D plotCoords;
    int plotsPerSide = 8;
    int exp, obs;

    chunkCoords = {0,0};
    plotCoords = {0,0};
    exp = 0;
    obs = makeID(chunkCoords, plotCoords, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    chunkCoords = {0,0};
    plotCoords = {0,7};
    exp = 56;
    obs = makeID(chunkCoords, plotCoords, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    chunkCoords = {0,0};
    plotCoords = {7,0};
    exp = 7;
    obs = makeID(chunkCoords, plotCoords, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    chunkCoords = {0,0};
    plotCoords = {7,7};
    exp = 63;
    obs = makeID(chunkCoords, plotCoords, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    chunkCoords = {-3,3};
    plotCoords = {5,4};
    exp = 3109;
    obs = makeID(chunkCoords, plotCoords, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testIdToPlotCoords()
{
    bool passed = true;
    std::cout << "\nTesting idToPlotCoords()" << std::endl;

    int plotID;
    Point2D exp, obs;
    int plotsPerSide = 8;

    plotID = 0;
    exp = {0,0};
    obs = idToPlotCoords(plotID, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED when plot ID = " << plotID << std::endl;
    }

    plotID = 7;
    exp = {7,0};
    obs = idToPlotCoords(plotID, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED when plot ID = " << plotID << std::endl;
    }

    plotID = 56;
    exp = {0,7};
    obs = idToPlotCoords(plotID, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED when plot ID = " << plotID << std::endl;
    }

    plotID = 63;
    exp = {7,7};
    obs = idToPlotCoords(plotID, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED when plot ID = " << plotID << std::endl;
    }

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testIDAboveBelowLeftRight()
{
    bool passed = true;
    std::cout << "\nTesting idAbove(), idBelow(), idLeft(), idRight()" << std::endl;

    int id;
    int plotsPerSide = 8;
    int exp, obs;

    id = 0;
    obs = idAbove(id, plotsPerSide);
    exp = 248;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on above when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idBelow(id, plotsPerSide);
    exp = 8;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on below when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idLeft(id, plotsPerSide);
    exp = 71;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on left when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idRight(id, plotsPerSide);
    exp = 1;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on right when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    id = 29;
    obs = idAbove(id, plotsPerSide);
    exp = 21;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on above when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idBelow(id, plotsPerSide);
    exp = 37;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on below when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idLeft(id, plotsPerSide);
    exp = 28;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on left when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idRight(id, plotsPerSide);
    exp = 30;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on right when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    id = 248;
    obs = idAbove(id, plotsPerSide);
    exp = 240;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on above when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idBelow(id, plotsPerSide);
    exp = 0;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on below when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idLeft(id, plotsPerSide);
    exp = 191;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on left when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    obs = idRight(id, plotsPerSide);
    exp = 249;
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED on right when ID = " << id << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

bool testGetIDofNearestPlot()
{
    bool passed = true;
    std::cout << "\nTesting getIDofNearestPlot" << std::endl;

    Point p;
    int chunkSize = 512;
    int plotsPerSide = 8;
    int obs, exp;

    // ----------- The Plot 0 ------------
    exp = 0;
    // In the center of the plot
    p = {32, 0, 32};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for center of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the top left of the plot
    p = {8, 0, 8};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for top left of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the top right of the plot
    p = {56, 0, 8};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for top right of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the bottom left of the plot
    p = {8, 0, 56};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for bottom left of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the bottom right of the plot
    p = {56, 0, 56};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for bottom right of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    // ---------- The Plot 71 ----------
    exp = 71;
    // In the center of the plot
    p = {-32, 0, 32};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for center of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the top left of the plot
    p = {-56, 0, 8};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for top left of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the top right of the plot
    p = {-8, 0, 8};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for top right of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the bottom left of the plot
    p = {-56, 0, 56};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for bottom left of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the bottom right of the plot
    p = {-8, 0, 56};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for bottom right of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    // ---------- The Plot 191 ----------
    exp = 191;
    // In the center of the plot
    p = {-32, 0, -32};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for center of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the top left of the plot
    p = {-56, 0, -56};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for top left of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the top right of the plot
    p = {-8, 0, -56};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for top right of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the bottom left of the plot
    p = {-56, 0, -8};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for bottom left of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }
    // In the bottom right of the plot
    p = {-8, 0, -8};
    obs = getIDofNearestPlot(p, chunkSize, plotsPerSide);
    if(exp != obs)
    {
        passed = false;
        std::cout << "Test FAILED for bottom right of plot" << std::endl;
        std::cout << "Expected " << exp << ", observed " << obs << std::endl;
    }

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}