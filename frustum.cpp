#include "frustum.h"

Frustum::Frustum() : Solid()
{
    upperXWidth = 15;
    upperZWidth = 15;
    initializeCorners();
    initializeLinePoints();
}
Frustum::Frustum(Point inputCenter, RGBAcolor inputColor,
                 double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                 double inputUpperXWidth, double inputUpperZWidth, linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor, inputLinesDrawn)
{
    upperXWidth = inputUpperXWidth;
    upperZWidth = inputUpperZWidth;
    initializeCorners();
    initializeLinePoints();
}
Frustum::Frustum(Point inputCenter, RGBAcolor inputColor,
                 double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                 double inputUpperXWidth, double inputUpperZWidth,
                 Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
                 Point inputOwnerCenter, linesDrawnEnum inputLinesDrawn)
                 : Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                                 inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter,
                                                 inputLinesDrawn)
{
    upperXWidth = inputUpperXWidth;
    upperZWidth = inputUpperZWidth;
    initializeCorners();
    initializeLinePoints();
}

void Frustum::initializeCorners()
{
    corners.push_back({center.x + upperXWidth/2, center.y + yWidth/2, center.z + upperZWidth/2});
    corners.push_back({center.x - upperXWidth/2, center.y + yWidth/2, center.z + upperZWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + upperXWidth/2, center.y + yWidth/2, center.z - upperZWidth/2});
    corners.push_back({center.x - upperXWidth/2, center.y + yWidth/2, center.z - upperZWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
}

void Frustum::initializeLinePoints()
{
    initializeXLinePoints();
    initializeYLinePoints();
    initializeZLinePoints();
}

void Frustum::initializeXLinePoints()
{
    int numPoints;
    double x,y,z;

    double width = fmax(xWidth, upperXWidth);
    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(width / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(width / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(width / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    // The x lines (circling around the x-axis)
    double distanceBetweenPointsXBottom = xWidth / numPoints;
    double distanceBetweenPointsXTop = upperXWidth / numPoints;

    // Top front line
    x = corners[1].x;
    y = corners[1].y + lineOffset;
    z = corners[1].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsXTop;
        xLinePoints.push_back({x, y, z});
    }

    // Bottom Front line
    x = corners[3].x;
    y = corners[3].y - lineOffset;
    z = corners[3].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsXBottom;
        xLinePoints.push_back({x, y, z});
    }

    // Bottom back line
    x = corners[7].x;
    y = corners[7].y - lineOffset;
    z = corners[7].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsXBottom;
        xLinePoints.push_back({x, y, z});
    }

    // Top Back line
    x = corners[5].x;
    y = corners[5].y + lineOffset;
    z = corners[5].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsXTop;
        xLinePoints.push_back({x, y, z});
    }
}
void Frustum::initializeYLinePoints()
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
    y = corners[3].y;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        x = center.x - getXWidthAtHeight(y) - lineOffset;
        z = center.z + getZWidthAtHeight(y) + lineOffset;
        yLinePoints.push_back({x, y, z});
    }

    // Front Right line
    y = corners[2].y;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        x = center.x + getXWidthAtHeight(y) + lineOffset;
        z = center.z + getZWidthAtHeight(y) + lineOffset;
        yLinePoints.push_back({x, y, z});
    }

    // Back right line
    y = corners[6].y;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        x = center.x - getXWidthAtHeight(y) - lineOffset;
        z = center.z + getZWidthAtHeight(y) + lineOffset;
        yLinePoints.push_back({x, y, z});
    }

    // Back Left line
    y = corners[7].y;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        x = center.x - getXWidthAtHeight(y) - lineOffset;
        z = center.z - getZWidthAtHeight(y) - lineOffset;
        yLinePoints.push_back({x, y, z});
    }
}
void Frustum::initializeZLinePoints()
{
    int numPoints;
    double x,y,z;

    double width = fmax(zWidth, upperZWidth);
    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(width / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(width / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(width / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    // The z lines (circling around the z-axis)
    double distanceBetweenPointsZBottom = zWidth / numPoints;
    double distanceBetweenPointsZTop = upperZWidth / numPoints;

    // Top Right line
    x = corners[4].x + lineOffset;
    y = corners[4].y + lineOffset;
    z = corners[4].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZTop;
        zLinePoints.push_back({x, y, z});
    }

    // Bottom right line
    x = corners[6].x + lineOffset;
    y = corners[6].y - lineOffset;
    z = corners[6].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZBottom;
        zLinePoints.push_back({x, y, z});
    }

    // Bottom Left line
    x = corners[7].x - lineOffset;
    y = corners[7].y - lineOffset;
    z = corners[7].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZBottom;
        zLinePoints.push_back({x, y, z});
    }

    // Top Left line
    x = corners[5].x - lineOffset;
    y = corners[5].y + lineOffset;
    z = corners[5].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZTop;
        zLinePoints.push_back({x, y, z});
    }
}


double Frustum::getXWidthAtHeight(double y) const
{
    double slope = (upperXWidth - xWidth) / 2 / yWidth;
    return xWidth/2 + slope*y;
}

double Frustum::getZWidthAtHeight(double y) const
{
    double slope = (upperZWidth - zWidth) / 2 / yWidth;
    return zWidth/2 + slope*y;
}


void Frustum::lookAt(Point &p)
{

}

void Frustum::draw() const
{
    drawLines();
    drawFaces();
}

void Frustum::drawLines() const
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

void Frustum::drawFaces() const
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

void Frustum::drawGridLines() const
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

std::experimental::optional<Point> Frustum::correctCollision(Point p, int buffer)
{
    return correctFrustumCollision(p, buffer, center, xWidth, yWidth, zWidth, upperXWidth, upperZWidth);
}

void Frustum::printDebugStats()
{
    std::cout<<"Frustum at " << center.x << "," << center.y << "," << center.z << std::endl;
    std::cout << "xWidth: " << xWidth << std::endl;
    std::cout << "yWidth: " << xWidth << std::endl;
    std::cout << "zWidth: " << xWidth << std::endl;
    std::cout << "upperxWidth: " << upperXWidth << std::endl;
    std::cout << "upperyWidth: " << upperZWidth << std::endl;
}

std::experimental::optional<Point> correctFrustumCollision(Point p, int buffer, Point c,
                                                           double xw, double yw, double zw, double topxw, double topzw)
{
    double distanceAboveTop = p.y - c.y - yw/2;
    double distanceBelowBottom = c.y - yw/2 - p.y;
    double frontDistanceNearTop = p.z - c.z - topzw/2;
    double rightDistanceNearTop = p.x - c.x - topxw/2;
    double backDistanceNearTop = c.z - topzw/2 - p.z;
    double leftDistanceNearTop = c.x - topxw/2 - p.x;
    double frontDistanceNearBottom = p.z - c.z - zw/2;
    double rightDistanceNearBottom = p.x - c.x - xw/2;
    double backDistanceNearBottom = c.z - zw/2 - p.z;
    double leftDistanceNearBottom = c.x - xw/2 - p.x;

    bool isWithinBordersNearTop = frontDistanceNearTop < buffer && rightDistanceNearTop < buffer &&
            backDistanceNearTop < buffer && leftDistanceNearTop < buffer;
    bool isWithinBordersNearBottom = frontDistanceNearBottom < buffer && rightDistanceNearBottom < buffer &&
                                  backDistanceNearBottom < buffer && leftDistanceNearBottom < buffer;
    // If the point is above the top
    if(distanceAboveTop > 0 && isWithinBordersNearTop)
    {
        if(distanceAboveTop >= buffer)
        {
            return std::experimental::nullopt;
        }
        else
        {
            return std::experimental::optional<Point>({p.x, c.y + yw/2 + buffer, p.z});
        }
    }
    // If the point is below the bottom
    else if(distanceBelowBottom > 0 && isWithinBordersNearBottom)
    {
        if(distanceBelowBottom >= buffer)
        {
            return std::experimental::nullopt;
        }
        else
        {
            return std::experimental::optional<Point>({p.x, c.y - yw/2 - buffer, p.z});
        }
    }
    else
    {
        double deltaY = p.y - (c.y - yw/2); // how far the point is above the bottom of the solid
        if(deltaY < 0 || deltaY > yw)
        {
            return std::experimental::nullopt;
        }
        double xwAtHeight = xw + deltaY/yw * (topxw - xw);
        double zwAtHeight = zw + deltaY/yw * (topzw - zw);
        return correctRectangularCrossSection(p, buffer, c, xwAtHeight, zwAtHeight);
    }
}