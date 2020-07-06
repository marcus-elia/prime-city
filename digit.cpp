#include "digit.h"

std::vector<std::vector<int>>Digit::segmentsVector{{1,2,3,4,5,6},{2,3},{1,2,7,5,4},
                                                   {1,2,7,3,4},{6,7,2,3},{1,6,7,3,4},
                                                   {1,4,3,5,6,7},{1,2,3},{1,2,3,4,5,6,7},
                                                   {1,2,3,4,6,7}};

Digit::Digit()
{
    digit = 0;
    center = {0, 10, 0};
    color = {1,1,1,1};
    width = 10;
    height = 20;
    thickness = 4;
    xzAngle = PI/2; // Face toward positive z by default
    initializeCorners();
}

Digit::Digit(Point inputCenter, RGBAcolor inputColor, Point &inputOwnerCenter, int inputDigit,
             double inputWidth, double inputHeight, double inputThickness)
{
    if(inputDigit >= 0)
    {
        digit = inputDigit;
    }
    else
    {
        digit = -inputDigit;
    }
    center = inputCenter;
    color = inputColor;
    ownerCenter = inputOwnerCenter;
    width = inputWidth;
    height = inputHeight;
    thickness = inputThickness;
    xzAngle = PI/2; // Face toward positive z by default
    initializeCorners();
}

void Digit::initializeCorners()
{
    for(int i : segmentsVector[digit])
    {
        switch(i)
        {
            case 1: // draw segment number 1, the top horizontal segment
                corners.push_back({center.x - width/2 + thickness/2, center.y + height/2 - thickness/2, center.z});
                corners.push_back({center.x - width/2 + thickness, center.y + height/2 - thickness, center.z});
                corners.push_back({center.x - width/2 + thickness, center.y + height/2, center.z});
                corners.push_back({center.x + width/2-thickness, center.y + height/2, center.z});
                corners.push_back({center.x + width/2-thickness, center.y + height/2 - thickness, center.z});
                corners.push_back({center.x + width/2 - thickness/2, center.y + height/2 - thickness/2, center.z});
                break;
            case 2:
                corners.push_back({center.x + width/2 - thickness/2, center.y + height/2 - thickness/2, center.z});
                corners.push_back({center.x + width/2 - thickness, center.y + height/2 - thickness, center.z});
                corners.push_back({center.x + width/2, center.y + height/2 - thickness, center.z});
                corners.push_back({center.x + width/2 - thickness, center.y + thickness/2, center.z});
                corners.push_back({center.x + width/2, center.y + thickness/2, center.z});
                corners.push_back({center.x + width/2 - thickness/2, center.y, center.z});
                break;
            case 3:
                corners.push_back({center.x + width/2 - thickness/2, center.y, center.z});
                corners.push_back({center.x + width/2 - thickness, center.y - thickness/2, center.z});
                corners.push_back({center.x + width/2, center.y - thickness/2, center.z});
                corners.push_back({center.x + width/2 - thickness, center.y - height/2 + thickness, center.z});
                corners.push_back({center.x + width/2, center.y - height/2 + thickness, center.z});
                corners.push_back({center.x + width/2 - thickness/2, center.y - height/2 + thickness/2, center.z});
                break;
            case 4:
                corners.push_back({center.x - width/2 + thickness/2, center.y - height/2 + thickness/2, center.z});
                corners.push_back({center.x - width/2 + thickness, center.y - height/2, center.z});
                corners.push_back({center.x - width/2 + thickness, center.y - height/2 + thickness, center.z});
                corners.push_back({center.x + width/2-thickness, center.y - height/2, center.z});
                corners.push_back({center.x + width/2-thickness, center.y - height/2 + thickness, center.z});
                corners.push_back({center.x + width/2 - thickness/2, center.y - height/2 + thickness/2, center.z});
                break;
            case 5:
                corners.push_back({center.x - width/2 + thickness/2, center.y, center.z});
                corners.push_back({center.x - width/2 + thickness, center.y - thickness/2, center.z});
                corners.push_back({center.x - width/2, center.y - thickness/2, center.z});
                corners.push_back({center.x - width/2 + thickness, center.y - height/2 + thickness, center.z});
                corners.push_back({center.x - width/2, center.y - height/2 + thickness, center.z});
                corners.push_back({center.x - width/2 + thickness/2, center.y - height/2 + thickness/2, center.z});
                break;
            case 6:
               corners.push_back({center.x - width/2 + thickness/2, center.y + height/2 - thickness/2, center.z});
               corners.push_back({center.x - width/2 + thickness, center.y + height/2 - thickness, center.z});
               corners.push_back({center.x - width/2, center.y + height/2 - thickness, center.z});
               corners.push_back({center.x - width/2 + thickness, center.y + thickness/2, center.z});
               corners.push_back({center.x - width/2, center.y + thickness/2, center.z});
               corners.push_back({center.x - width/2 + thickness/2, center.y, center.z});
                break;
            case 7:
                corners.push_back({center.x - width/2 + thickness/2, center.y, center.z});
                corners.push_back({center.x - width/2 + thickness, center.y - thickness/2, center.z});
                corners.push_back({center.x - width/2 + thickness, center.y + thickness/2, center.z});
                corners.push_back({center.x + width/2-thickness, center.y + thickness/2, center.z});
                corners.push_back({center.x + width/2-thickness, center.y - thickness/2, center.z});
                corners.push_back({center.x + width/2 - thickness/2, center.y, center.z});
                break;
            default:
                break;
        }
    }
}

void Digit::setColor(RGBAcolor inputColor)
{
    color = inputColor;
}
void Digit::setXZAngle(double inputAngle)
{
    double rotationAmount = inputAngle - xzAngle;
    rotate(0, rotationAmount, 0);
}

void Digit::draw() const
{
    glColor4f(color.r, color.g, color.b, color.a);
    for(int i = 0; i < corners.size()/6; i++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(int j = 0; j < 6; j++)
        {
            int index = 6*i + j;
            glVertex3f(corners[index].x, corners[index].y, corners[index].z);
        }
        glEnd();
    }
}




void Digit::move(double deltaX, double deltaY, double deltaZ)
{
    center.x += deltaX;
    center.y += deltaY;
    center.z += deltaZ;
    for(Point &p : corners)
    {
        p.x += deltaX;
        p.y += deltaY;
        p.z += deltaZ;
    }
}
void Digit::moveSelfAndOwner(double deltaX, double deltaY, double deltaZ)
{
    move(deltaX, deltaY, deltaZ);
    ownerCenter.x += deltaX;
    ownerCenter.y += deltaY;
    ownerCenter.z += deltaZ;
}
void Digit::rotate(double thetaX, double thetaY, double thetaZ)
{
    for(Point &p : corners)
    {
        rotatePointAroundPoint(p, center, thetaX, thetaY, thetaZ);
    }
    xzAngle += thetaY;
}
void Digit::rotateAroundOwner(double thetaX, double thetaY, double thetaZ)
{
    for(Point &p : corners)
    {
        rotatePointAroundPoint(p, ownerCenter, thetaX, thetaY, thetaZ);
    }
    rotatePointAroundPoint(center, ownerCenter, thetaX, thetaY, thetaZ);
}