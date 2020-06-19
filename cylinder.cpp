#include "cylinder.h"

Cylinder::Cylinder() : Solid()
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    initializeCorners();
    initializeLinePoints();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                   linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor, inputLinesDrawn)
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    initializeCorners();
    initializeLinePoints();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                   double inputTopXWidth, double inputTopZWidth, linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor, inputLinesDrawn)
{
    topXWidth = inputTopXWidth;
    topZWidth = inputTopZWidth;
    initializeCorners();
    initializeLinePoints();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                   Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
                   Point inputOwnerCenter, linesDrawnEnum inputLinesDrawn) :
                   Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                                   inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter,
                                                   inputLinesDrawn)
{
    topXWidth = xWidth;
    topZWidth = zWidth;
    initializeCorners();
    initializeLinePoints();
}
Cylinder::Cylinder(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                   double inputTopXWidth, double inputTopZWidth,
                   Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
                   Point inputOwnerCenter, linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
              inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter, inputLinesDrawn)
{
    topXWidth = inputTopXWidth;
    topZWidth = inputTopZWidth;
    initializeCorners();
    initializeLinePoints();
}


void Cylinder::initializeCorners()
{
    double x, z, xTop, zTop;
    for(int i = 0; i < smoothness; i++)
    {
        x = xWidth/2 * cos(2*PI* i / smoothness);
        z = zWidth/2 * sin(2*PI* i / smoothness);
        xTop = topXWidth/2 * cos(2*PI* i / smoothness);
        zTop = topZWidth/2 * sin(2*PI* i / smoothness);
        corners.push_back({center.x + xTop, center.y + yWidth/2, center.z + zTop}); // upper face
        corners.push_back({center.x + x, center.y - yWidth/2, center.z + z}); // lower face
    }
}

void Cylinder::initializeLinePoints()
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

    double distanceBetweenPoints = yWidth / numPoints;

    // Iterate through the bottom of the ellipse and add points above it
    for(int i = 1; i < corners.size(); i += 2)
    {
        y = 0;

        double theta = 2*PI* (i+1)/2 / smoothness;

        linePoints.emplace_back();
        for(int j = 0; j < numPoints - 1; j++)
        {
            y += distanceBetweenPoints;
            linePoints.back().push_back(getPointAtHeight(y, theta));
        }
    }
}



void Cylinder::lookAt(Point &p)
{

}

double Cylinder::getXRadiusAtHeight(double y) const
{
    double slope = (topXWidth - xWidth) / 2 / yWidth;
    return xWidth/2 + slope*y;
}

double Cylinder::getZRadiusAtHeight(double y) const
{
    double slope = (topZWidth - zWidth) / 2 / yWidth;
    return zWidth/2 + slope*y;
}

Point Cylinder::getPointAtHeight(double y, double theta) const
{
    double xRad = getXRadiusAtHeight(y);
    double zRad = getZRadiusAtHeight(y);
    return {center.x + xRad*cos(theta), center.y - yWidth/2 + y, center.z + zRad*sin(theta)};
}



void Cylinder::draw() const
{
    drawLines();
    drawFaces();
}

void Cylinder::drawLines() const
{
    if(linesDrawn == NoLines)
    {
        return;
    }

    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);
    for(int i = 0; i < 2*smoothness - 3; i += 2)
    {
        // Upper face
        drawPoint(corners[i]);
        drawPoint(corners[i + 2]);

        // Lower face
        drawPoint(corners[i + 1]);
        drawPoint(corners[i + 3]);

        // Verticals
        if(((linesDrawn == Low || linesDrawn == Normal) && i % 12 == 0) || (linesDrawn == Medium && i % 6 == 0) ||
        linesDrawn == High)
        {
            drawPoint(corners[i]);
            drawPoint(corners[i+1]);
        }
    }
    // Connect the end to the start
    drawPoint(corners[2*smoothness-2]);
    drawPoint(corners[0]);
    drawPoint(corners[2*smoothness-1]);
    drawPoint(corners[1]);

    // Vertical
    if(linesDrawn == High)
    {
        drawPoint(corners[2*smoothness-2]);
        drawPoint(corners[2*smoothness-1]);
    }

    glEnd();

    if(linesDrawn != Normal)
    {
        drawGridLines();
    }
}

void Cylinder::drawFaces() const
{
    glColor4f(color.r, color.g, color.b, color.a);
    glDisable(GL_CULL_FACE);

    // Draw the top and bottom circles
    glBegin(GL_TRIANGLE_FAN);
    // center
    drawPoint({center.x, center.y + yWidth/2, center.z});
    // top circumference
    for(int i = 0; i < smoothness; i++)
    {
        drawPoint(corners[2*i]);
    }
    drawPoint(corners[0]);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    // center
    drawPoint({center.x, center.y - yWidth/2, center.z});
    // bottom circumference
    for(int i = 0; i < smoothness; i++)
    {
        drawPoint(corners[2*i + 1]);
    }
    drawPoint(corners[1]);
    glEnd();

    // Draw the cylinder part

    glBegin(GL_TRIANGLE_STRIP);

    for(int i = 0; i < 2*smoothness; i++)
    {
        drawPoint(corners[i]);
    }
    // Connect the end to the start
    drawPoint(corners[0]);
    drawPoint(corners[1]);
    glEnd();

    glEnable(GL_CULL_FACE);
}

void Cylinder::drawGridLines() const
{
    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);

    int pointsPerSide;

    pointsPerSide = linePoints[0].size();
    for(int i = 0; i < pointsPerSide; i++)
    {
        drawPoint(linePoints[0][i]);
        for(int j = 1; j < smoothness; j++)
        {
            drawPoint(linePoints[j][i]);
            drawPoint(linePoints[j][i]);
        }
        drawPoint(linePoints[0][i]);
    }

    glEnd();
}




std::experimental::optional<Point> Cylinder::correctCollision(Point p, int buffer)
{
    if(hasBeenRotated)
    {
        throw std::domain_error("This Cylinder has been rotated. Cannot perform collision detection.");
    }
    return correctCylinderCollision(p, buffer, center,
            xWidth, yWidth, zWidth,
            topXWidth, topZWidth);
}

void Cylinder::printDebugStats()
{
    std::cout<<"Cylinder at " << center.x << "," << center.y << "," << center.z << std::endl;
    std::cout << "xWidth: " << xWidth << std::endl;
    std::cout << "yWidth: " << xWidth << std::endl;
    std::cout << "zWidth: " << xWidth << std::endl;
}

double focusDistanceSum(Point p, Point c, double xw, double zw)
{
    double focalLength;
    Point f1, f2;              // focii
    if(xw < zw) // z-width is bigger
    {
        focalLength = sqrt(zw*zw - xw*xw)/2;
        f1 = {c.x, 0, c.z + focalLength};
        f2 = {c.x, 0, c.z - focalLength};
    }
    else
    {
        focalLength = sqrt(xw*xw - zw*zw)/2;
        f1 = {c.x + focalLength, 0, c.z};
        f2 = {c.x - focalLength, 0, c.z};
    }

    return distance2d(p, f1) + distance2d(p, f2);
}

std::experimental::optional<Point> correctCylinderCollision(Point p, int buffer, Point c,
                                                            double xw, double yw, double zw,
                                                            double topxw, double topzw)
{
    double distanceAboveTop = p.y - c.y - yw/2;
    double distanceBelowBottom = c.y - yw/2 - p.y;
    double focusDistanceNearTop = focusDistanceSum(p, c, topxw, topzw);
    double focusDistanceNearBottom = focusDistanceSum(p, c, xw, zw);
    if((distanceAboveTop > focusDistanceNearTop  && distanceAboveTop > distanceBelowBottom) ||
            (distanceAboveTop > 0 && focusDistanceNearTop < fmax(topxw, topzw)))
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
    else if((distanceBelowBottom > focusDistanceNearBottom  && distanceBelowBottom > distanceAboveTop) ||
            (distanceBelowBottom > 0 && focusDistanceNearBottom < fmax(xw, zw)))
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
        return correctEllipticalCrossSection(p, buffer, c, xwAtHeight, zwAtHeight);
    }
}


std::experimental::optional<Point> correctEllipticalCrossSection(Point p, int buffer, Point c,
                                                                 double xw, double zw)
{
    if(xw == 0 || zw == 0) // If the ellipse is just a line
    {
        return std::experimental::nullopt;
    }

    double a, b, focalLength;  // a is the larger radius, b is the smaller radius
    Point f1, f2;              // focii
    if(xw < zw) // z-width is bigger
    {
        a = zw/2, b = xw/2;
        focalLength = sqrt(zw*zw - xw*xw)/2;
        f1 = {c.x, 0, c.z + focalLength};
        f2 = {c.x, 0, c.z - focalLength};
    }
    else
    {
        a = xw/2, b = zw/2;
        focalLength = sqrt(xw*xw - zw*zw)/2;
        f1 = {c.x + focalLength, 0, c.z};
        f2 = {c.x - focalLength, 0, c.z};
    }

    double focusDistanceSum = distance2d(p, f1) + distance2d(p, f2);

    // If the point is far enough away, don't change it
    if(focusDistanceSum >= 2*a + buffer)
    {
        return std::experimental::nullopt;
    }
    else // Otherwise, move it away
    {
        // the x and z coordinates, pretending the ellipse was centered at (0,0)
        double x = p.x - c.x;
        double z = p.z - c.z;

        // special cases to avoid division by 0
        if(z == 0) // If on the x-axis, send it right or left
        {
            if(x > 0)
            {
                return std::experimental::optional<Point>({c.x + xw/2 + buffer, c.y, c.z});
            }
            else
            {
                return std::experimental::optional<Point>({c.x - xw/2 - buffer, c.y, c.z});
            }
        }
        else if(x == 0) // If on the z-axis, go straight up or down
        {
            if(z > 0)
            {
                return std::experimental::optional<Point>({c.x, c.y, c.z + zw/2 + buffer});
            }
            else
            {
                return std::experimental::optional<Point>({c.x, c.y, c.z - zw/2 - buffer});
            }
        }
        else  // Otherwise, we need to use the derivative of the ellipse equation
        {
            // First, project (x,z) onto the boundary of the ellipse
            // We have already handled cases of x = 0, z = 0, a = 0, b = 0
            double xbar, zbar;
            if(x > 0)
            {
                xbar = b / sqrt((z*z)/(x*x) + (b*b)/(a*a));
            }
            else
            {
                xbar = -b / sqrt((z*z)/(x*x) + (b*b)/(a*a));
            }

            zbar = (z/x)*xbar;


            // Calculate the derivative at that point
            // We know xbar != +/- a because that would require x = 0, and
            // that case already returned a value
            // And |xbar| < |a| since a is the major axis
            double derivative;
            if(z > 0)
            {
                derivative = (b/a)*(-xbar) / sqrt(a*a - xbar*xbar);
            }
            else
            {
                derivative = (b/a)*xbar / sqrt(a*a - xbar*xbar);
            }


            // Find the angle of inclination of the normal line
            double normalSlope = -1/derivative;
            double angle = atan(normalSlope);

            // Put the new point at the buffer using sin/cos of angle
            double correctedX, correctedZ;
            if(x > 0)
            {
                correctedX =  c.x + xbar + buffer*cos(angle);
                correctedZ = c.z + zbar + buffer*sin(angle);
            }
            else
            {
                correctedX =  c.x + xbar - buffer*cos(angle);
                correctedZ = c.z + zbar - buffer*sin(angle);
            }
            return std::experimental::optional<Point>({correctedX, p.y, correctedZ});
        }

    }
}