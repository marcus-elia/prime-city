#include "capsule.h"

Capsule::Capsule() : Solid()
{
    pointsPerRing = 16;
    numRings = 9;
    initializeCorners();
}
Capsule::Capsule(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    pointsPerRing = 16;
    numRings = 9;
    initializeCorners();
}
Capsule::Capsule(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
int inputNumRings, int inputPointsPerRing) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor)
{
    pointsPerRing = inputPointsPerRing;
    numRings = inputNumRings;
    initializeCorners();
}
Capsule::Capsule(Point inputCenter, RGBAcolor inputColor,
double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
Point inputLocation, Point inputLookingAt, double inputSpeed, Point inputVelocity,
Point inputOwnerCenter) : Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor,
                                inputLocation, inputLookingAt, inputSpeed, inputVelocity, inputOwnerCenter)
{
    pointsPerRing = 16;
    numRings = 9;
    initializeCorners();
}

void Capsule::initializeCorners()
{
    // corners[0] = top
    // corners[1] = bottom
    // corners[(i-1)*pointsPerRing + 2] = start of ith ring (starting at top)
    // But we add an extra ring because of the cylinder
    // corners has pointsPerRing*(numRings+1) + 2 entries
    corners.push_back({center.x, center.y + yWidth/2, center.z});
    corners.push_back({center.x, center.y - yWidth/2, center.z});

    int numRingsPerHalf = (numRings + 1) / 2;
    double cylinderHeight = yWidth - xWidth;

    double curY;
    double phi = -PI/2;
    double deltaPhi = (PI/2) / numRingsPerHalf;
    double rx, rz;  // the x and z radii at the given height

    // Top hemisphere
    double centerY = center.y + cylinderHeight/2;
    for(int ring = 0; ring < numRingsPerHalf; ring++)
    {
        phi += deltaPhi;
        curY = centerY +  1 / sqrt(4 / (tan(phi)*tan(phi)*xWidth*xWidth) + 4 / (xWidth*xWidth));

        rx = xWidth/2 * sqrt(1 - 4*(curY-centerY)*(curY-centerY)/xWidth/xWidth);
        rz = zWidth/2 * sqrt(1 - 4*(curY-centerY)*(curY-centerY)/xWidth/xWidth);
        double theta;
        for(int i = 0; i < pointsPerRing; i++)
        {
            theta = (double)i / pointsPerRing * 2 * PI;
            corners.push_back({center.x + rx*cos(theta), curY, center.z + rz*sin(theta)});
        }
    }
    // Bottom hemisphere
    centerY = center.y - cylinderHeight/2;
    for(int ring = 0; ring < numRingsPerHalf; ring++)
    {
        phi += deltaPhi;
        curY = centerY -  1 / sqrt(4 / (tan(phi)*tan(phi)*xWidth*xWidth) + 4 / (xWidth*xWidth));

        rx = xWidth/2 * sqrt(1 - 4*(curY-centerY)*(curY-centerY)/xWidth/xWidth);
        rz = zWidth/2 * sqrt(1 - 4*(curY-centerY)*(curY-centerY)/xWidth/xWidth);
        double theta;
        for(int i = 0; i < pointsPerRing; i++)
        {
            theta = (double)i / pointsPerRing * 2 * PI;
            corners.push_back({center.x + rx*cos(theta), curY, center.z + rz*sin(theta)});
        }
    }
}

void Capsule::lookAt(Point &p)
{

}

void Capsule::draw() const
{
    glDisable(GL_CULL_FACE);
    //drawLines();
    drawFaces();
    glEnable(GL_CULL_FACE);
}

void Capsule::drawLines() const
{
    glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
    glBegin(GL_LINES);

    // The top
    for(int i = 0; i < pointsPerRing; i++)
    {
        drawPoint(corners[0]);
        drawPoint(corners[i+2]);
    }
    // The Bottom
    for(int i = 0; i < pointsPerRing; i++)
    {
        drawPoint(corners[1]);
        drawPoint(corners[(numRings)*pointsPerRing + 2 + i]);
    }
    // First ring, horiztonal lines
    for(int i = 0; i < pointsPerRing; i++)
    {
        drawPoint(corners[i + 2]);
        drawPoint(corners[2 + (i + 1) % pointsPerRing]);
    }

    // Iterate through the other rings
    for(int ring = 1; ring < numRings + 1; ring++)
    {
        for(int i = 0; i < pointsPerRing; i++)
        {
            // Vertical segment
            drawPoint(corners[(ring-1)*pointsPerRing + 2 + i]); // the higher (previous) ring
            drawPoint(corners[ring*pointsPerRing + 2 + i]);     // the lower (current) ring

            // Horizontal segment
            drawPoint(corners[ring*pointsPerRing + 2 + i]);
            drawPoint(corners[ring*pointsPerRing + 2 + (i + 1) % pointsPerRing]);
        }
    }
    glEnd();
}
void Capsule::drawFaces() const
{
    glColor4f(color.r, color.g, color.b, color.a);

    // Top
    glBegin(GL_TRIANGLE_FAN);
    drawPoint(corners[0]);
    for(int i = 2; i < pointsPerRing + 2; i++)
    {
        drawPoint(corners[i]);
    }
    drawPoint(corners[2]);
    glEnd();

    // Bottom
    glBegin(GL_TRIANGLE_FAN);
    drawPoint(corners[1]);
    for(int i = 2; i < pointsPerRing + 2; i++)
    {
        drawPoint(corners[(numRings)*pointsPerRing + i]);
    }
    drawPoint(corners[(numRings)*pointsPerRing + 2]);
    glEnd();

    // Connect consecutive rings
    for(int ring = 1; ring < numRings + 1; ring++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i < pointsPerRing; i++)
        {
            drawPoint(corners[(ring-1)*pointsPerRing + 2 + i]); // the higher (previous) ring
            drawPoint(corners[ring*pointsPerRing + 2 + i]);     // the lower (current) ring
        }
        // Connect to the start points
        drawPoint(corners[(ring-1)*pointsPerRing + 2]);
        drawPoint(corners[ring*pointsPerRing + 2]);
        glEnd();
    }
}

std::experimental::optional<Point> Capsule::correctCollision(Point p, int buffer)
{
    return std::experimental::nullopt;
}

void Capsule::printDebugStats()
{

}
