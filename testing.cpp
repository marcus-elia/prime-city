#include "recPrism.h"
#include <experimental/optional>

bool testIsAboveLine();

bool testCorrectRectangularCrossSection();


int main()
{
    testIsAboveLine();
    testCorrectRectangularCrossSection();
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

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}