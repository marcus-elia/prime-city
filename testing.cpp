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



bool testCorrectRectangularCrossSection()
{
    bool passed = true;
    std::cout << "\nTesting correctRectangularCrossSection()" << std::endl;

    const double TOLERANCE = 0.01;
    Point p, c;
    double xw, zw;
    int buffer;
    std::experimental::optional<Point> exp, obs;

    // When the Point does not need to be corrected
    exp = std::experimental::nullopt;
    c = {0, 0, 0};
    xw = 5;
    zw = 10;
    buffer = 2;

    // Way too far left
    p = {-30, 0, 0};
    obs = correctRectangularCrossSection(p, buffer, c, xw, zw);
    if(obs)
    {
        passed = false;
        std::cout << "FAILED test of point way too far left." << std::endl;
    }


    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}