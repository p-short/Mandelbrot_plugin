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
    for (int i = 0; i < thing.size() -1 ; i++)
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
    magVecA = clampIt(magnitude(vecA), 0 , 1);
}

bool Sphere::checkIntersection(double &rotatingArm, bool other)
{
    scaledPos = createCoord(x * 160, y * 160);
//    Coord midPoint = createCoord(0, 0);
    rp = createCoord(160 * cos(rotatingArm), 160 * sin(rotatingArm));
    
    newVecA = createVector(scaledPos, mp);
//    Coord vecB = createVector(rotPoint, midPoint);
    nVecB = normalise(createVector(rp, mp));
    
    scalarProjection = clampIt(dotProduct(newVecA, nVecB), 0, 160);
    
    spx = scalarProjection * cos(rotatingArm);
    spy = scalarProjection * sin(rotatingArm);
    
    dist0 = distance(rp.x, rp.y, scaledPos.x, scaledPos.y);
    dist1 = distance(scaledPos.x, scaledPos.y, spx, spy);
    dist2 = distance(scaledPos.x, scaledPos.y, mp.x, mp.y);

    if (dist0 < 160 && dist1 < sphereRadius && other && dist2 > sphereRadius)
    {
        return true;
    }
    
    else if (dist0 < 160 && dist1 > sphereRadius)
    {
        isIntersecting = true;
        return false;
    }
    
    else
    {
        return false;
    }
}

void Sphere::setSphereBool(bool myBool)
{
    isIntersecting = myBool;
}

bool Sphere::getSphereBool()
{
    return isIntersecting;
}

float Sphere::getXPos()
{
    x = magVecA * cos(phi);
    return x;
}

float Sphere::getYPos()
{
    y = magVecA * sin(phi);
    return y;
}

void Sphere::paint (juce::Graphics& g)
{
//    x = magVecA * cos(phi);
//    y = magVecA * sin(phi);
//    g.setOrigin(getWidth() / 2 - 10, getHeight() / 2 - 10 + 20);
//    g.setColour(juce::Colour::fromFloatRGBA (1.0f, 0.0f, 0.0f, 0.5f));
//    g.fillEllipse(x * 160, y * 160, sphereRadius * 2, sphereRadius * 2);

}


void Sphere::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

