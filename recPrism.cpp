#include "recPrism.h"

RecPrism::RecPrism() : Solid()
{
    initializeCorners();
    initializeLinePoints();
}
RecPrism::RecPrism(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                   linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor, inputLinesDrawn)
{
    initializeCorners();
    initializeLinePoints();
}

RecPrism::RecPrism(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                   Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
                   Point inputOwnerCenter, linesDrawnEnum inputLinesDrawn) :
                   Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                         inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter, inputLinesDrawn)
{
    initializeCorners();
    initializeLinePoints();
}

void RecPrism::initializeCorners()
{
    corners.push_back({center.x + xWidth/2, center.y + yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y + yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y + yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y + yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
}

void RecPrism::initializeLinePoints()
{
    initializeXLinePoints();
    initializeYLinePoints();
    initializeZLinePoints();
}

void RecPrism::initializeXLinePoints()
{
    int numPoints;
    double x,y,z;

    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(xWidth / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(xWidth / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(xWidth / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    // The x lines (circling around the x-axis)
    double distanceBetweenPointsX = xWidth / numPoints;

    // Top front line
    x = corners[1].x;
    y = corners[1].y + lineOffset;
    z = corners[1].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsX;
        xLinePoints.push_back({x, y, z});
    }

    // Bottom Front line
    x = corners[3].x;
    y = corners[3].y - lineOffset;
    z = corners[3].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsX;
        xLinePoints.push_back({x, y, z});
    }

    // Bottom back line
    x = corners[7].x;
    y = corners[7].y - lineOffset;
    z = corners[7].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsX;
        xLinePoints.push_back({x, y, z});
    }

    // Top Back line
    x = corners[5].x;
    y = corners[5].y + lineOffset;
    z = corners[5].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsX;
        xLinePoints.push_back({x, y, z});
    }
}

void RecPrism::initializeYLinePoints()
{
    int numPoints;
    double x,y,z;

    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(yWidth / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(yWidth / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(yWidth / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    // The y lines (circling around the y-axis)
    double distanceBetweenPointsY = yWidth / numPoints;

    // Front left line
    x = corners[3].x - lineOffset;
    y = corners[3].y;
    z = corners[3].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        yLinePoints.push_back({x, y, z});
    }

    // Front Right line
    x = corners[2].x + lineOffset;
    y = corners[2].y;
    z = corners[2].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        yLinePoints.push_back({x, y, z});
    }

    // Back right line
    x = corners[6].x + lineOffset;
    y = corners[6].y;
    z = corners[6].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        yLinePoints.push_back({x, y, z});
    }

    // Back Left line
    x = corners[7].x - lineOffset;
    y = corners[7].y;
    z = corners[7].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        yLinePoints.push_back({x, y, z});
    }
}

void RecPrism::initializeZLinePoints()
{
    int numPoints;
    double x,y,z;

    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(zWidth / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(zWidth / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(zWidth / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    // The z lines (circling around the z-axis)
    double distanceBetweenPointsZ = zWidth / numPoints;

    // Top Right line
    x = corners[4].x + lineOffset;
    y = corners[4].y + lineOffset;
    z = corners[4].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZ;
        zLinePoints.push_back({x, y, z});
    }

    // Bottom right line
    x = corners[6].x + lineOffset;
    y = corners[6].y - lineOffset;
    z = corners[6].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZ;
        zLinePoints.push_back({x, y, z});
    }

    // Bottom Left line
    x = corners[7].x - lineOffset;
    y = corners[7].y - lineOffset;
    z = corners[7].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZ;
        zLinePoints.push_back({x, y, z});
    }

    // Top Left line
    x = corners[5].x - lineOffset;
    y = corners[5].y + lineOffset;
    z = corners[5].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZ;
        zLinePoints.push_back({x, y, z});
    }
}



void RecPrism::lookAt(Point &p)
{

}

void RecPrism::draw() const
{
    glDisable(GL_CULL_FACE);
    drawLines();
    drawFaces();
    glEnable(GL_CULL_FACE);

    // Debugging: print a ring around the middle of each rectangle to
    // prove that it knows where it is.
    /*glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0,1,0,1);
    drawPoint(center);
    double x, z;
    for(int i = 0; i < 7; i++)
    {
        x = center.x + xWidth*cos(i*2*PI/6);
        z = center.z + zWidth*sin(i*2*PI/6);
        drawPoint({x, center.y, z});
    }
    glEnd();
    glEnable(GL_CULL_FACE);*/
}

void RecPrism::drawLines() const
{
    if(linesDrawn == NoLines)
    {
        return;
    }

    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);
    drawPoint(corners[1]);
    drawPoint(corners[0]);

    drawPoint(corners[1]);
    drawPoint(corners[3]);

    drawPoint(corners[3]);
    drawPoint(corners[2]);

    drawPoint(corners[2]);
    drawPoint(corners[0]);

    drawPoint(corners[4]);
    drawPoint(corners[5]);

    drawPoint(corners[5]);
    drawPoint(corners[7]);

    drawPoint(corners[7]);
    drawPoint(corners[6]);

    drawPoint(corners[6]);
    drawPoint(corners[4]);

    drawPoint(corners[0]);
    drawPoint(corners[4]);

    drawPoint(corners[2]);
    drawPoint(corners[6]);

    drawPoint(corners[3]);
    drawPoint(corners[7]);

    drawPoint(corners[1]);
    drawPoint(corners[5]);

    glEnd();

    if(linesDrawn != Normal)
    {
        drawGridLines();
    }
}

void RecPrism::drawFaces() const
{
    glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);

    drawPoint(corners[0]);
    drawPoint(corners[1]);
    drawPoint(corners[3]);
    drawPoint(corners[2]);

    drawPoint(corners[5]);
    drawPoint(corners[4]);
    drawPoint(corners[6]);
    drawPoint(corners[7]);

    drawPoint(corners[6]);
    drawPoint(corners[4]);
    drawPoint(corners[0]);
    drawPoint(corners[2]);

    drawPoint(corners[4]);
    drawPoint(corners[5]);
    drawPoint(corners[1]);
    drawPoint(corners[0]);

    drawPoint(corners[6]);
    drawPoint(corners[2]);
    drawPoint(corners[3]);
    drawPoint(corners[7]);

    drawPoint(corners[3]);
    drawPoint(corners[1]);
    drawPoint(corners[5]);
    drawPoint(corners[7]);
    glEnd();
}

void RecPrism::drawGridLines() const
{
    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);

    int pointsPerSide;

    // x lines
    pointsPerSide = xLinePoints.size()/4;
    for(int i = 0; i < pointsPerSide; i++)
    {
        drawPoint(xLinePoints[i]);
        drawPoint(xLinePoints[i + pointsPerSide]);
        drawPoint(xLinePoints[i + pointsPerSide]);
        drawPoint(xLinePoints[i + 2*pointsPerSide]);
        drawPoint(xLinePoints[i + 2*pointsPerSide]);
        drawPoint(xLinePoints[i + 3*pointsPerSide]);
        drawPoint(xLinePoints[i + 3*pointsPerSide]);
        drawPoint(xLinePoints[i]);
    }

    // y lines
    pointsPerSide = yLinePoints.size()/4;
    for(int i = 0; i < pointsPerSide; i++)
    {
        drawPoint(yLinePoints[i]);
        drawPoint(yLinePoints[i + pointsPerSide]);
        drawPoint(yLinePoints[i + pointsPerSide]);
        drawPoint(yLinePoints[i + 2*pointsPerSide]);
        drawPoint(yLinePoints[i + 2*pointsPerSide]);
        drawPoint(yLinePoints[i + 3*pointsPerSide]);
        drawPoint(yLinePoints[i + 3*pointsPerSide]);
        drawPoint(yLinePoints[i]);
    }

    // z lines
    pointsPerSide = zLinePoints.size()/4;
    for(int i = 0; i < pointsPerSide; i++)
    {
        drawPoint(zLinePoints[i]);
        drawPoint(zLinePoints[i + pointsPerSide]);
        drawPoint(zLinePoints[i + pointsPerSide]);
        drawPoint(zLinePoints[i + 2*pointsPerSide]);
        drawPoint(zLinePoints[i + 2*pointsPerSide]);
        drawPoint(zLinePoints[i + 3*pointsPerSide]);
        drawPoint(zLinePoints[i + 3*pointsPerSide]);
        drawPoint(zLinePoints[i]);
    }
}

// Wrapper function
std::experimental::optional<Point> RecPrism::correctCollision(Point p, int buffer)
{
    if(hasBeenRotated)
    {
        throw std::domain_error("This RecPrism has been rotated. Cannot perform collision detection.");
    }
    return correctRectangularPrismCollision(p, buffer, center,
                                            xWidth, yWidth, zWidth);
}

void RecPrism::printDebugStats()
{
    std::cout<<"Rectangular Prism at " << center.x << "," << center.y << "," << center.z << std::endl;
    std::cout << "xWidth: " << xWidth << std::endl;
    std::cout << "yWidth: " << xWidth << std::endl;
    std::cout << "zWidth: " << xWidth << std::endl;
}

// Static
std::experimental::optional<Point> correctRectangularPrismCollision(Point p, int buffer, Point c,
                                                                    double xw, double yw, double zw)
{
    double distanceOutsideLeftEdge = c.x - xw/2 - p.x;
    double distanceOutsideRightEdge = p.x - c.x - xw/2;
    double distanceAboveTopEdge = p.y - c.y - yw/2;
    double distanceBelowBottomEdge = c.y - yw/2 - p.y;
    double distanceOutsideFrontEdge = p.z - c.z - zw/2;
    double distanceOutsideBackEdge = c.z - zw/2 - p.z;
    // If the point is closest to the top face
    if(distanceAboveTopEdge > distanceOutsideLeftEdge && distanceAboveTopEdge > distanceOutsideRightEdge &&
    distanceAboveTopEdge > distanceOutsideFrontEdge && distanceAboveTopEdge > distanceOutsideBackEdge)
    {
        if(distanceAboveTopEdge > buffer)
        {
            return std::experimental::nullopt;
        }
        else
        {
            return std::experimental::optional<Point>({p.x, c.y + yw/2 + buffer, p.z});
        }
    }
    // If the point is closest to the bottom face
    else if(distanceBelowBottomEdge > distanceOutsideLeftEdge && distanceBelowBottomEdge > distanceOutsideRightEdge &&
            distanceBelowBottomEdge > distanceOutsideFrontEdge && distanceBelowBottomEdge > distanceOutsideBackEdge)
    {
        if(distanceBelowBottomEdge > buffer)
        {
            return std::experimental::nullopt;
        }
        else
        {
            return std::experimental::optional<Point>({p.x, c.y - yw/2 - buffer, p.z});
        }
    }
    // Otherwise, assume it is near a side face and correct that
    else
    {
        return correctRectangularCrossSection(p, buffer, c, xw, zw);
    }
}


/*
 * + - - - - - +
 * |  \  1  /  |
 * | 4   X   2 |
 * |  /  3  \  |
 * + - - - - - +
 */
std::experimental::optional<Point> correctRectangularCrossSection(Point p, int buffer, Point c,
                                                                  double xw, double zw)
{
    // Determine which of the 4 zones of the rectangle p lies in
    // Let line 1 have the positive slope, and line 2 have the negative slope
    // Line 1: z = mx + b1
    // Line 2: z = -mx + b2
    double m = zw / xw;
    double b1 = c.x - m*c.z;
    double b2 = c.x + m*c.z;
    bool above1 = p.isAboveLine(m, b1);
    bool above2 = p.isAboveLine(-m, b2);
    if(above1 && above2) // zone 1
    {
        if(p.z > c.z - zw/2 - buffer)
        {
            return std::experimental::optional<Point>({p.x, p.y, c.z - zw/2 - buffer});
        }
    }
    else if(above1) // zone 4
    {
        if(p.x > c.x - xw/2 - buffer)
        {
            return std::experimental::optional<Point>({p.x - zw/2 - buffer, p.y, c.z});
        }
    }
    else if(above2) // zone 2
    {
        if(p.x < c.x + xw/2 + buffer)
        {
            return std::experimental::optional<Point>({p.x + zw/2 + buffer, p.y, c.z});
        }
    }
    else // zone 3
    {
        if(p.z > c.x + zw/2 + buffer)
        {
            return std::experimental::optional<Point>({p.x, p.y, c.z + zw/2 + buffer});
        }
    }
    return std::experimental::nullopt;
}