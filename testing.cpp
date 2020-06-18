#include "recPrism.h"
#include "cylinder.h"
#include "frustum.h"
#include <experimental/optional>

bool testIsAboveLine();

bool testCorrectRectangularCrossSection();

bool testCorrectRectangularPrismCollision();

bool testCorrectEllipticalCrossSection();


int main()
{
    testIsAboveLine();
    testCorrectRectangularCrossSection();
    testCorrectRectangularPrismCollision();
    testCorrectEllipticalCrossSection();
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