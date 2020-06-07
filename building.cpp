#include "building.h"

Building::Building()
{
    topLeft = {0,0};
    sideLength = 32;

    height = 200;
    color = {0,0.4,1,1};
    edgeColor = {.9,.9,.9, 1};

    buildingType = Plain;

    initializeSolids();
}
Building::Building(Point2D inputTopLeft, int inputSideLength, int inputHeight,
                   RGBAcolor inputColor, RGBAcolor inputEdgeColor, typeOfBuilding inputBuildingType)
{
    topLeft = inputTopLeft;
    sideLength = inputSideLength;
    height = inputHeight;
    color = inputColor;
    edgeColor = inputEdgeColor;
    buildingType = inputBuildingType;

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
    else if(r < 80)
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
    if(shapeSeed < 60)
    {
        solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                            sideLength, height, sideLength, edgeColor, lineDensity)));
    }
    // Then try a frustum
    else if(shapeSeed < 80)
    {
        double upperXWidth = sideLength - (rand() % (sideLength/2));
        double upperZWidth = sideLength - (rand() % (sideLength/2));
        solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                           sideLength, height, sideLength, edgeColor,
                                                           upperXWidth, upperZWidth, lineDensity)));
    }
    // Finally a cylinder
    else
    {
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
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
    }
    else
    {
        baseHeight = height/2;
        center = {(double)topLeft.x + sideLength/2, (double)baseHeight/2, (double)topLeft.z + sideLength/2};

        // Make a frustum that fits the rec prism base
        if(baseShapeSeed < 10)
        {
            solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                                 sideLength, baseHeight, sideLength, edgeColor, lineDensity)));
            center = {center.x, center.y + baseHeight, center.z};
            solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                               sideLength, baseHeight, sideLength, edgeColor,
                                                               sideLength/2, sideLength/2, lineDensity)));
        }
        // Otherwise, the base and the top are independent
        else
        {
            // Make the base
            if(baseShapeSeed < 50)
            {
                solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                                     sideLength, baseHeight, sideLength, edgeColor, lineDensity)));
            }
            else if(baseShapeSeed < 75)
            {
                solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                                     sideLength, baseHeight, sideLength, edgeColor,
                                                                     sideLength, sideLength, lineDensity)));
            }
            else
            {
                solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
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
                solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color, xWidth, baseHeight, zWidth,
                                                                     edgeColor, lineDensity)));
            }
            else if(topShapeSeed < 70) // Cylinder
            {
                xWidth = sideLength/2 - (rand() % (sideLength/4));
                zWidth = sideLength/2 - (rand() % (sideLength/4));
                topXWidth = sideLength/2 - (rand() % (sideLength/4));
                topZWidth = sideLength/2 - (rand() % (sideLength/4));
                solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                                     xWidth, baseHeight, zWidth, edgeColor,
                                                                     topXWidth, topZWidth, lineDensity)));
            }
            else // Frustum
            {
                xWidth = sideLength/2 - (rand() % (sideLength/4));
                zWidth = sideLength/2 - (rand() % (sideLength/4));
                topXWidth = sideLength/2 - (rand() % (sideLength/4));
                topZWidth = sideLength/2 - (rand() % (sideLength/4));
                solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
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
            solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                                 xWidth, solidHeight, zWidth, edgeColor, lineDensity)));
            center.y += solidHeight;
            xWidth = 3*sideLength/4;
            zWidth = 3*sideLength/4;
            solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                                 xWidth, solidHeight,
                                                                 zWidth, edgeColor, lineDensity)));
            center.y += solidHeight;
            xWidth = sideLength/2;
            zWidth = sideLength/2;
            solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                                 xWidth, solidHeight,
                                                                 zWidth, edgeColor, lineDensity)));
        }
        else if(shapeSeed < 75) // cylinder
        {
            xWidth = sideLength;
            zWidth = sideLength;
            solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                                 xWidth, solidHeight, zWidth, edgeColor, lineDensity)));
            center.y += solidHeight;
            xWidth = 3*sideLength/4;
            zWidth = 3*sideLength/4;
            solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                                 xWidth, solidHeight,
                                                                 zWidth, edgeColor, lineDensity)));
            center.y += solidHeight;
            xWidth = sideLength/2;
            zWidth = sideLength/2;
            solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                                 xWidth, solidHeight,
                                                                 zWidth, edgeColor, lineDensity)));
        }
        else // Frustum
        {
            xWidth = sideLength;
            zWidth = sideLength;
            topXWidth = 7*sideLength/8;
            topZWidth = 7*sideLength/8;
            solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                                 xWidth, solidHeight, zWidth, edgeColor,
                                                                 topXWidth, topZWidth, lineDensity)));
            center.y += solidHeight;
            xWidth = 3*sideLength/4;
            zWidth = 3*sideLength/4;
            topXWidth = 5*sideLength/8;
            topZWidth = 5*sideLength/8;
            solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                               xWidth, solidHeight, zWidth, edgeColor,
                                                               topXWidth, topZWidth, lineDensity)));
            center.y += solidHeight;
            xWidth = sideLength/2;
            zWidth = sideLength/2;
            topXWidth = 3*sideLength/8;
            topZWidth = 3*sideLength/8;
            solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                               xWidth, solidHeight, zWidth, edgeColor,
                                                               topXWidth, topZWidth, lineDensity)));
        }
    }
}
void Building::initializeSolids4()
{
    initializeSolids1();
}

linesDrawnEnum Building::getRandomLineDensity() const
{
    int lineSeed = rand() % 100;
    if(lineSeed < 20)
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









// ===================================
//
//             Getters
//
// ===================================

std::vector<std::shared_ptr<Solid>> Building::getSolids() const
{
    return solids;
}
typeOfBuilding Building::getBuildingType() const
{
    return buildingType;
}

void Building::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
}

std::experimental::optional<Point> Building::correctCollision(Point p, int buffer)
{
    for(std::shared_ptr<Solid> s : solids)
    {
        std::experimental::optional<Point> newPoint = s->correctCollision(p, buffer);
        if(newPoint)
        {
            return newPoint;
        }
    }
    return std::experimental::nullopt;
}