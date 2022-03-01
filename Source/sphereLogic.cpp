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
    ap_cp = createCoord(x, y);
    ap_mp = createCoord(0, 0);
    ap_vecA = createVector(ap_cp, ap_mp); // point 1 to center point
    
    ap_phi = atan2(y, x);
    
    if (isnan(ap_phi))
    {
        ap_phi = M_PI * 2;
    }
    
    ap_magVecA = clampIt(magnitude(ap_vecA), 0 , 1);
    
    if (isnan(ap_magVecA))
    {
        ap_magVecA = 1.0;
    }
    
    ap_newX = ap_magVecA * cos(ap_phi);
    ap_newY = ap_magVecA * sin(ap_phi);
}

bool SphereLogic::checkIntersection(double &rotatingArm, bool other)
{
    ap_scaledPos = createCoord(ap_newX * (ap_bordRad - ap_sphereRadius),
                               ap_newY * (ap_bordRad - ap_sphereRadius));
    
    ap_rp = createCoord(ap_bordRad * cos(rotatingArm), ap_bordRad * sin(rotatingArm));
    
    ap_newVecA = createVector(ap_scaledPos, ap_mp);
    ap_nVecB = normalise(createVector(ap_rp, ap_mp));
    
    ap_scalarProjection = clampIt(dotProduct(ap_newVecA, ap_nVecB), 0, ap_bordRad - ap_sphereRadius);
    
    ap_spx = ap_scalarProjection * cos(rotatingArm);
    ap_spy = ap_scalarProjection * sin(rotatingArm);
    
    ap_dist0 = distance(ap_rp.x, ap_rp.y, ap_scaledPos.x, ap_scaledPos.y);
    ap_dist1 = distance(ap_scaledPos.x, ap_scaledPos.y, ap_spx, ap_spy);
    ap_dist2 = distance(ap_scaledPos.x, ap_scaledPos.y, ap_mp.x, ap_mp.y);
    
//    if (this.dist0 < borderRad && this.dist1 < this.r && other &&
//        this.dist2 > this.r) {
//        return true;
//    } else if (this.dist0 < borderRad && this.dist1 > this.r) {
    
    if (ap_dist0 < ap_bordRad && ap_dist1 < ap_sphereRadius && other && ap_dist2 > ap_sphereRadius)
    {
        return true;
    }
    
    else if (ap_dist0 < ap_bordRad && ap_dist1 > ap_sphereRadius)
    {
        ap_isIntersecting = true;
        return false;
    }
    
    else
    {
        return false;
    }
}

void SphereLogic::setSphereBool(bool myBool)
{
    ap_isIntersecting = myBool;
}

bool SphereLogic::getSphereBool()
{
    return ap_isIntersecting;
}

float SphereLogic::getXPos()
{
    return ap_newX;
}

float SphereLogic::getYPos()
{
    return ap_newY;
}

float SphereLogic::getMag()
{
    return ap_magVecA;
}
