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
    int lineSeed = rand() % 100;
    linesDrawnEnum lineDensity;
    if(lineSeed < 40)
    {
        lineDensity = Normal;
    }
    else if(lineSeed < 60)
    {
        lineDensity = Low;
    }
    else if(lineSeed < 80)
    {
        lineDensity = Medium;
    }
    else
    {
        lineDensity = High;
    }

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
                                                           upperXWidth, upperZWidth, High)));
    }
    // Finally a cylinder
    else
    {
        double upperXWidth = sideLength - (rand() % (sideLength/2));
        double upperZWidth = sideLength - (rand() % (sideLength/2));
        solids.push_back(std::make_shared<Cylinder>(Cylinder(center, color,
                                                           sideLength, height, sideLength, edgeColor,
                                                           upperXWidth, upperZWidth, High)));
    }
}
void Building::initializeSolids2()
{
    initializeSolids1();
}
void Building::initializeSolids3()
{
    initializeSolids1();
}
void Building::initializeSolids4()
{
    initializeSolids1();
}

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