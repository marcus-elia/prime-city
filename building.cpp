#include "building.h"

Building::Building()
{
    topLeft = {0,0};
    sideLength = 32;

    height = 200;
    color = {0,0.4,1,1};
    edgeColor = {.9,.9,.9, 1};

    initializeSolids();
}
Building::Building(Point2D inputTopLeft, int inputSideLength, int inputHeight,
                   RGBAcolor inputColor, RGBAcolor inputEdgeColor)
{
    topLeft = inputTopLeft;
    sideLength = inputSideLength;
    height = inputHeight;
    color = inputColor;
    edgeColor = inputEdgeColor;

    initializeSolids();

}

void Building::initializeSolids()
{
    int r = rand() % 100;
    if(r < 40)
    {
        initializeSolids1();
    }
    else if(r < 60)
    {
        initializeSolids2();
    }
    else if(r < 90)
    {
        initializeSolids3();
    }
    else
    {
        initializeSolids4();
    }
}


void Building::initializeSolids1()
{
    // Determine the gridlines
    linesDrawnEnum lineDensity = getRandomLineDensity();

    RGBAcolor gray = {0.5, 0.5, 0.5, 1};
    Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};

    // Determine the shape
    int shapeSeed = rand() % 100;
    // Make a rectangle most of the time
    if(shapeSeed < 65)
    {
        solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color,
                                                            sideLength, height, sideLength, edgeColor, lineDensity)));
    }
    // Then try a frustum
    else if(shapeSeed < 85)
    {
        double upperXWidth = sideLength - (rand() % (sideLength/2));
        double upperZWidth = sideLength - (rand() % (sideLength/2));
        solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                           sideLength, height, sideLength, edgeColor,
                                                           upperXWidth, upperZWidth, lineDensity)));
    }
    // Finally a cylinder
    else
    {
        solids.push_back(std::unique_ptr<Cylinder>(new Cylinder(center, color,
                                                           sideLength, height, sideLength, edgeColor,
                                                           sideLength, sideLength, lineDensity)));
    }
}
void Building::initializeSolids2()
{
    // Is the top piece just a spire or not
    bool spire = (rand() % 100) > 50;

    // Determine the shape of the base
    int baseShapeSeed = rand() % 100;
    // Determine the shape of the top
    int topShapeSeed = rand() % 100;

    // Determine the gridlines
    linesDrawnEnum lineDensity = getRandomLineDensity();

    RGBAcolor gray = {0.5, 0.5, 0.5, 1};
    Point center;

    int baseHeight;
    if(spire)
    {
        baseHeight = 3*height/4;
        int centerY = baseHeight/2;
        addRandomSolid(centerY, baseHeight, sideLength, sideLength, sideLength, sideLength, 50, 75, lineDensity);
        centerY = height;
        // Spire
        addRandomSolid(centerY, height/2, sideLength/8, sideLength/8, sideLength/9, sideLength/9, 50, 75, lineDensity);
    }
    else
    {
        baseHeight = height/2;
        center = {(double)topLeft.x + sideLength/2, (double)baseHeight/2, (double)topLeft.z + sideLength/2};

        // Make a frustum that fits the rec prism base
        if(baseShapeSeed < 20)
        {
            solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color,
                                                                 sideLength, baseHeight, sideLength, edgeColor, lineDensity)));
            center = {center.x, center.y + baseHeight, center.z};
            solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                               sideLength, baseHeight, sideLength, edgeColor,
                                                               sideLength/2, sideLength/2, lineDensity)));
        }
        // Otherwise, the base and the top are independent
        else
        {
            // Make the base
            if(baseShapeSeed < 50)
            {
                solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color,
                                                                     sideLength, baseHeight, sideLength, edgeColor, lineDensity)));
            }
            else if(baseShapeSeed < 75)
            {
                solids.push_back(std::unique_ptr<Cylinder>(new Cylinder(center, color,
                                                                     sideLength, baseHeight, sideLength, edgeColor,
                                                                     sideLength, sideLength, lineDensity)));
            }
            else
            {
                solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                                     sideLength, baseHeight, sideLength, edgeColor,
                                                                     sideLength/2, sideLength/2, lineDensity)));
            }

            // Make the top
            center = {center.x, center.y + baseHeight, center.z};
            double xWidth, zWidth, topXWidth, topZWidth;
            if(topShapeSeed < 35) // Rectangle
            {
                xWidth = sideLength/2 - (rand() % (sideLength/4));
                zWidth = sideLength/2 - (rand() % (sideLength/4));
                solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color, xWidth, baseHeight, zWidth,
                                                                     edgeColor, lineDensity)));
            }
            else if(topShapeSeed < 70) // Cylinder
            {
                xWidth = sideLength/2 - (rand() % (sideLength/4));
                zWidth = sideLength/2 - (rand() % (sideLength/4));
                topXWidth = sideLength/2 - (rand() % (sideLength/4));
                topZWidth = sideLength/2 - (rand() % (sideLength/4));
                solids.push_back(std::unique_ptr<Cylinder>(new Cylinder(center, color,
                                                                     xWidth, baseHeight, zWidth, edgeColor,
                                                                     topXWidth, topZWidth, lineDensity)));
            }
            else // Frustum
            {
                xWidth = sideLength/2 - (rand() % (sideLength/4));
                zWidth = sideLength/2 - (rand() % (sideLength/4));
                topXWidth = sideLength/2 - (rand() % (sideLength/4));
                topZWidth = sideLength/2 - (rand() % (sideLength/4));
                solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                                     xWidth, baseHeight, zWidth, edgeColor,
                                                                     topXWidth, topZWidth, lineDensity)));
            }
        }
    }
}
void Building::initializeSolids3()
{
    // Determine the gridlines
    linesDrawnEnum lineDensity = getRandomLineDensity();

    RGBAcolor gray = {0.5, 0.5, 0.5, 1};
    Point center;
    int xWidth, zWidth, topXWidth, topZWidth;

    int structureSeed = rand() % 100;
    // Three of the same type, stacked
    if(structureSeed < 30)
    {
        int solidHeight = height/3;
        center = {(double)topLeft.x + sideLength/2, (double)solidHeight/2, (double)topLeft.z + sideLength/2};
        int shapeSeed = rand() % 100;
        if(shapeSeed < 50) // rectangular
        {
            xWidth = sideLength;
            zWidth = sideLength;
            solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color,
                                                                 xWidth, solidHeight, zWidth, edgeColor, lineDensity)));
            center.y += solidHeight;
            xWidth = 3*sideLength/4;
            zWidth = 3*sideLength/4;
            solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color,
                                                                 xWidth, solidHeight,
                                                                 zWidth, edgeColor, lineDensity)));
            center.y += solidHeight;
            xWidth = sideLength/2;
            zWidth = sideLength/2;
            solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color,
                                                                 xWidth, solidHeight,
                                                                 zWidth, edgeColor, lineDensity)));
        }
        else if(shapeSeed < 75) // cylinder
        {
            xWidth = sideLength;
            zWidth = sideLength;
            solids.push_back(std::unique_ptr<Cylinder>(new Cylinder(center, color,
                                                                 xWidth, solidHeight, zWidth, edgeColor, lineDensity)));
            center.y += solidHeight;
            xWidth = 3*sideLength/4;
            zWidth = 3*sideLength/4;
            solids.push_back(std::unique_ptr<Cylinder>(new Cylinder(center, color,
                                                                 xWidth, solidHeight,
                                                                 zWidth, edgeColor, lineDensity)));
            center.y += solidHeight;
            xWidth = sideLength/2;
            zWidth = sideLength/2;
            solids.push_back(std::unique_ptr<Cylinder>(new Cylinder(center, color,
                                                                 xWidth, solidHeight,
                                                                 zWidth, edgeColor, lineDensity)));
        }
        else // Frustum
        {
            xWidth = sideLength;
            zWidth = sideLength;
            topXWidth = 7*sideLength/8;
            topZWidth = 7*sideLength/8;
            solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                                 xWidth, solidHeight, zWidth, edgeColor,
                                                                 topXWidth, topZWidth, lineDensity)));
            center.y += solidHeight;
            xWidth = 3*sideLength/4;
            zWidth = 3*sideLength/4;
            topXWidth = 5*sideLength/8;
            topZWidth = 5*sideLength/8;
            solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                               xWidth, solidHeight, zWidth, edgeColor,
                                                               topXWidth, topZWidth, lineDensity)));
            center.y += solidHeight;
            xWidth = sideLength/2;
            zWidth = sideLength/2;
            topXWidth = 3*sideLength/8;
            topZWidth = 3*sideLength/8;
            solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                               xWidth, solidHeight, zWidth, edgeColor,
                                                               topXWidth, topZWidth, lineDensity)));
        }
    }
    else if(structureSeed < 65) // Three random pieces
    {
        // Divide the building into 3 heights

        // The chances of making each solid type
        int rectProb = 50;
        int cylProb = 75;

        // How tall to make the 3 parts
        int lowerDividingPoint = height/4 + (rand() % (height/2));
        int upperDividingPoint = lowerDividingPoint + (rand() % (height - 10 - lowerDividingPoint));

        int solidHeight, centerY;

        // Base
        solidHeight = lowerDividingPoint;
        centerY = solidHeight/2;
        xWidth = sideLength;
        zWidth = sideLength;
        addRandomSolid(centerY, solidHeight, xWidth, zWidth, xWidth, zWidth, rectProb, cylProb, lineDensity);

        // Middle
        solidHeight = upperDividingPoint - lowerDividingPoint;
        centerY = lowerDividingPoint + solidHeight/2;
        xWidth -= (rand() % (xWidth/2));
        zWidth -= (rand() % (zWidth/2));
        topXWidth = xWidth + (rand() % xWidth) - xWidth/3;
        topZWidth = zWidth + (rand() % zWidth) - zWidth/3;
        addRandomSolid(centerY, solidHeight, xWidth, zWidth, topXWidth, topZWidth, rectProb, cylProb, lineDensity);

        // Top
        solidHeight = height - upperDividingPoint;
        centerY = upperDividingPoint + solidHeight/2;
        xWidth -= (rand() % (xWidth/2));
        zWidth -= (rand() % (zWidth/2));
        topXWidth = xWidth + (rand() % xWidth) - xWidth/3;
        topZWidth = zWidth + (rand() % zWidth) - zWidth/3;
        addRandomSolid(centerY, solidHeight, xWidth, zWidth, topXWidth, topZWidth, rectProb, cylProb, lineDensity);
    }
    // A building with a complex top
    else
    {
        // The chances of making each solid type
        int rectProb = 50;
        int cylProb = 75;
        int solidHeight, centerY;

        // Base
        solidHeight = 3*height/4;
        centerY = solidHeight/2;
        xWidth = sideLength;
        zWidth = sideLength;
        topXWidth = sideLength - (rand() % (sideLength/4));
        topZWidth = sideLength - (rand() % (sideLength/4));
        addRandomSolid(centerY, solidHeight, xWidth, zWidth, topXWidth, topZWidth, rectProb, cylProb, lineDensity);

        // Middle
        solidHeight = height/8;
        centerY = 3*height/4 + solidHeight/2;
        xWidth = topXWidth - (rand() % (topXWidth/2));
        zWidth = topZWidth - (rand() % (topZWidth/2));
        topXWidth = xWidth + (rand() % (xWidth/2)) - xWidth/4;
        topZWidth = zWidth + (rand() % (zWidth/2)) - zWidth/4;
        addRandomSolid(centerY, solidHeight, xWidth, zWidth, topXWidth, topZWidth, rectProb, cylProb, lineDensity);

        // Top
        solidHeight = height/8;
        centerY += height/8;
        xWidth = sideLength - (rand() % (sideLength/2));
        zWidth = sideLength - (rand() % (sideLength/2));
        topXWidth = sideLength - (rand() % (sideLength/2));
        topZWidth = sideLength - (rand() % (sideLength/2));
        addRandomSolid(centerY, solidHeight, xWidth, zWidth, topXWidth, topZWidth, rectProb, cylProb, lineDensity);
    }
}
void Building::initializeSolids4()
{
    // Determine the gridlines
    linesDrawnEnum lineDensity = getRandomLineDensity();

    // Three random solids and a bonus on top
    // The chances of making each solid type
    int rectProb = 50;
    int cylProb = 75;

    int xWidth, zWidth, topXWidth, topZWidth;

    // How tall to make the 3 parts
    int lowerDividingPoint = height/4 + (rand() % (height/3));
    int upperDividingPoint = lowerDividingPoint + (rand() % (7*height/8 - lowerDividingPoint));

    int solidHeight, centerY;

    // Base
    solidHeight = lowerDividingPoint;
    centerY = solidHeight/2;
    xWidth = sideLength;
    zWidth = sideLength;
    addRandomSolid(centerY, solidHeight, xWidth, zWidth, xWidth, zWidth, rectProb, cylProb, lineDensity);

    // Middle
    solidHeight = upperDividingPoint - lowerDividingPoint;
    centerY = lowerDividingPoint + solidHeight/2;
    xWidth -= (rand() % (xWidth/2));
    zWidth -= (rand() % (zWidth/2));
    topXWidth = xWidth + (rand() % xWidth) - xWidth/3;
    topZWidth = zWidth + (rand() % zWidth) - zWidth/3;
    addRandomSolid(centerY, solidHeight, xWidth, zWidth, topXWidth, topZWidth, rectProb, cylProb, lineDensity);

    // Top
    solidHeight = 7*height/8 - upperDividingPoint;
    centerY = upperDividingPoint + solidHeight/2;
    xWidth -= (rand() % (xWidth/2));
    zWidth -= (rand() % (zWidth/2));
    topXWidth = xWidth + (rand() % xWidth) - xWidth/3;
    topZWidth = zWidth + (rand() % zWidth) - zWidth/3;
    addRandomSolid(centerY, solidHeight, xWidth, zWidth, topXWidth, topZWidth, rectProb, cylProb, lineDensity);

    // Bonus
    int bonusSeed = rand() % 100;
    solidHeight = height/8;
    xWidth = sideLength;
    zWidth = sideLength;
    if(bonusSeed < 45) // Full cylinder
    {
        Point center = {(double)topLeft.x + sideLength/2, 15*height/16.0, (double)topLeft.z + sideLength/2};
        solids.push_back(std::unique_ptr<Cylinder>(new Cylinder(center, color,
                                                             xWidth, solidHeight, zWidth, edgeColor, lineDensity)));
    }
    else if(bonusSeed < 90) // Rectangular spire
    {
        solidHeight = height/3;
        xWidth = sideLength/8;
        zWidth = sideLength/8;
        Point center = {(double)topLeft.x + sideLength/2, 7*height/8.0 + solidHeight/2, (double)topLeft.z + sideLength/2};
        solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color,
                                                             xWidth, solidHeight, zWidth, edgeColor, lineDensity)));
    }
    else // Frustum
    {
        topXWidth = sideLength/2;
        topZWidth = sideLength/2;
        Point center = {(double)topLeft.x + sideLength/2, 15*height/16.0, (double)topLeft.z + sideLength/2};
        solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                           xWidth, solidHeight, zWidth, edgeColor,
                                                           topXWidth, topZWidth, lineDensity)));
    }
}

linesDrawnEnum Building::getRandomLineDensity() const
{
    if(!GRIDLINES_ENABLED) // If gridlines are off
    {
        return Normal;
    }
    int lineSeed = rand() % 100;
    if(lineSeed < 40)
    {
        return Normal;
    }
    else if(lineSeed < 75)
    {
        return Low;
    }
    else if(lineSeed < 95)
    {
        return Medium;
    }
    else
    {
        return High;
    }
}


void Building::addRandomSolid(int centerHeight, int yWidth, int xWidth, int zWidth, int topXWidth, int topZWidth,
                    int rectPrismSeed, int cylinderSeed, linesDrawnEnum lineDensity)
{
    Point center = {(double)topLeft.x + sideLength/2, (double)centerHeight, (double)topLeft.z + sideLength/2};
    int shapeSeed = rand() % 100;
    if(shapeSeed < rectPrismSeed)
    {
        solids.push_back(std::unique_ptr<RecPrism>(new RecPrism(center, color,
                                                             xWidth, yWidth,
                                                             zWidth, edgeColor, lineDensity)));
    }
    else if(shapeSeed < cylinderSeed)
    {
        solids.push_back(std::unique_ptr<Cylinder>(new Cylinder(center, color,
                                                             xWidth, yWidth, zWidth, edgeColor,
                                                             topXWidth, topZWidth, lineDensity)));
    }
    else
    {
        solids.push_back(std::unique_ptr<Frustum>(new Frustum(center, color,
                                                             xWidth, yWidth, zWidth, edgeColor,
                                                             topXWidth, topZWidth, lineDensity)));
    }
}









// ===================================
//
//             Getters
//
// ===================================

std::vector<std::unique_ptr<Solid>> Building::getSolids() const
{
    return solids;
}

void Building::draw() const
{
    for(const std::unique_ptr<Solid> &s : solids)
    {
        s->draw();
    }
}

std::experimental::optional<Point> Building::correctCollision(Point p, int buffer)
{
    for(const std::unique_ptr<Solid> &s : solids)
    {
        std::experimental::optional<Point> newPoint = s->correctCollision(p, buffer);
        if(newPoint)
        {
            return newPoint;
        }
    }
    return std::experimental::nullopt;
}