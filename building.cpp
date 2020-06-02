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
    RGBAcolor gray = {0.5, 0.5, 0.5, 1};
    if(buildingType == Plain)
    {
        Point center = {(double)topLeft.x + sideLength/2, (double)height/2, (double)topLeft.z + sideLength/2};
        //solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
        //                                                     sideLength, height, sideLength, edgeColor, Medium)));
        solids.push_back(std::make_shared<Frustum>(Frustum(center, color,
                                                             sideLength, height, sideLength, edgeColor,
                                                             sideLength/2, sideLength/4, High)));
    }

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