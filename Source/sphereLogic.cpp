/*
  ==============================================================================

    sphereLogic.cpp
    Created: 19 Feb 2022 12:28:43pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#include "sphereLogic.h"

SphereLogic::SphereLogic()
{
    x = 0;
    y = 0;
    cx = 0;
    cy = 0;
}

SphereLogic::~SphereLogic()
{
    
}

void SphereLogic::setPosition(float _x, float _y, float _cx, float _cy)
{
    x = _x;
    y = _y;
    cx = _cx;
    cy = _cy;
}

void SphereLogic::updatePosition(std::vector<std::unique_ptr<SphereLogic> > &myVector)
{
    for (int i = 0; i < myVector.size() -1 ; i++)
    {
        myVector[i + 1]->x = (pow(myVector[i]->x, 2) + -pow(myVector[i]->y, 2)) +
        (pow(myVector[i]->cx, 2) + -pow(myVector[i]->cy, 2));
        myVector[i + 1]->y = 2 * myVector[i]->x * myVector[i]->y + 2 * myVector[i]->cx * myVector[i]->cy;
    }
}

void SphereLogic::limitSphere()
{
    //this code limits the sphere from leaving the boundry
    spherePoint = createCoord(x, y);
    centPoint = createCoord(0, 0);
    vecA = createVector(spherePoint, centPoint); // point 1 to center point
    
    phi = atan2(y, x);
    magVecA = clampIt(magnitude(vecA), 0 , 1);
}

bool SphereLogic::checkIntersection(double &rotatingArm, bool other)
{
    scaledPos = createCoord(x * 170, y * 170);
    //    Coord midPoint = createCoord(0, 0);
    rotationPoint = createCoord(170 * cos(rotatingArm), 170 * sin(rotatingArm));
    
    newVecA = createVector(scaledPos, centPoint);
    //    Coord vecB = createVector(rotPoint, midPoint);
    normVecB = normalise(createVector(rotationPoint, centPoint));
    
    scalarProjection = clampIt(dotProduct(newVecA, normVecB), 0, 160);
    
    spx = scalarProjection * cos(rotatingArm);
    spy = scalarProjection * sin(rotatingArm);
    
    dist0 = distance(rotationPoint.x, rotationPoint.y, scaledPos.x, scaledPos.y);
    dist1 = distance(scaledPos.x, scaledPos.y, spx, spy);
    dist2 = distance(scaledPos.x, scaledPos.y, centPoint.x, centPoint.y);
    
//    if (this.dist0 < borderRad && this.dist1 < this.r && other &&
//        this.dist2 > this.r) {
//        return true;
//    } else if (this.dist0 < borderRad && this.dist1 > this.r) {
    
    if (dist0 < 170 && dist1 < sphereRadius && other && dist2 > sphereRadius)
    {
        return true;
    }
    
    else if (dist0 < 170 && dist1 > sphereRadius)
    {
        isIntersecting = true;
        return false;
    }
    
    else
    {
        return false;
    }
}

void SphereLogic::setSphereBool(bool myBool)
{
    isIntersecting = myBool;
}

bool SphereLogic::getSphereBool()
{
    return isIntersecting;
}

float SphereLogic::getXPos()
{
    x = magVecA * cos(phi);
    return x;
}

float SphereLogic::getYPos()
{
    y = magVecA * sin(phi);
    return y;
}
