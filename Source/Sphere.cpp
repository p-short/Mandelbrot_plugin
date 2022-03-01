/*
  ==============================================================================

    Spheres.cpp
    Created: 29 Jan 2022 3:24:01pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Sphere.h"
#include "myFunctions.h"

//==============================================================================
Sphere::Sphere() //(float _x, float _y): x(_x), y(_y)
{
    x = 0;
    y = 0;
    cx = 0;
    cy = 0;
}

Sphere::~Sphere()
{
}

void Sphere::setPosition(float x_, float y_, float cx_, float cy_)
{
    x = x_;
    y = y_;
    cx = cx_;
    cy = cy_;
}

void Sphere::updatePosition(std::vector<std::unique_ptr<Sphere>>& thing)
{
    for (int i = 0; i < thing.size() - 1; i++)
    {
        thing[i + 1]->x = (pow(thing[i]->x, 2) + -pow(thing[i]->y, 2)) +
        (pow(thing[i]->cx, 2) + -pow(thing[i]->cy, 2));
        thing[i + 1]->y = 2 * thing[i]->x * thing[i]->y + 2 * thing[i]->cx * thing[i]->cy;
    }
}

void Sphere::limitSphere()
{
    //this code limits the sphere from leaving the boundry
    cp = createCoord(x, y);
    mp = createCoord(0, 0);
    vecA = createVector(cp, mp); // point 1 to center point
    
    phi = atan2(y, x);
    
    if (isnan(phi))
    {
        phi = M_PI * 2;
    }
    
    magVecA = clampIt(magnitude(vecA), 0 , 1);
    
    if (isnan(magVecA))
    {
        magVecA = 1.0;
    }
    
    newX = magVecA * cos(phi);
    newY = magVecA * sin(phi);
}

//bool Sphere::checkIntersection(double &rotatingArm, bool other)
//{
//    scaledPos = createCoord(x * 160, y * 160);
////    Coord newMP = createCoord(0, 0 + 20);
//    rp = createCoord(170 * cos(rotatingArm), 170 * sin(rotatingArm));
//    
//    newVecA = createVector(scaledPos, mp);
////    Coord vecB = createVector(rotPoint, midPoint);
//    nVecB = normalise(createVector(rp, mp));
//    
//    scalarProjection = clampIt(dotProduct(newVecA, nVecB), 0, 160);
//    
//    spx = scalarProjection * cos(rotatingArm);
//    spy = scalarProjection * sin(rotatingArm);
//    
//    dist0 = distance(rp.x, rp.y, scaledPos.x, scaledPos.y);
//    dist1 = distance(scaledPos.x, scaledPos.y, spx, spy);
//    dist2 = distance(scaledPos.x, scaledPos.y, mp.x, mp.y);
//
//    if (dist0 < 160 && dist1 < sphereRadius && other && dist2 > sphereRadius)
//    {
//        return true;
//    }
//    
//    else if (dist0 < 160 && dist1 > sphereRadius)
//    {
//        isIntersecting = true;
//        return false;
//    }
//    
//    else
//    {
//        return false;
//    }
//}

bool Sphere::checkForPaint(double inc)
{
    scaledPos = createCoord(newX * (bordRad - sphereRadius), newY * (bordRad - sphereRadius));
    rp = createCoord(bordRad * cos(inc), bordRad * sin(inc));
    nVecA = createVector(scaledPos, mp); // mp = (0, 0)
    vecB = createVector(rp, mp);
    nVecB = normalise(vecB);
    scalarProjection = clampIt(dotProduct(nVecA, nVecB), 0, bordRad - sphereRadius);
    
    spx = scalarProjection * cos(inc);
    spy = scalarProjection * sin(inc);
    
    dist0 = distance(rp.x, rp.y, scaledPos.x, scaledPos.y);
    dist1 = distance(scaledPos.x, scaledPos.y, spx, spy);
    dist2 = distance(scaledPos.x, scaledPos.y, mp.x, mp.y);
    
    return dist0 <= bordRad && dist1 < sphereRadius && dist2 > sphereRadius;
}

double Sphere::getXPos()
{
    return newX;
}

double Sphere::getYPos()
{
    return newY;
}

void Sphere::paint (juce::Graphics& g)
{
}

void Sphere::resized()
{
}

