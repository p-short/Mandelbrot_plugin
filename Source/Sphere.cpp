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

void Sphere::setPosition(float x_, float y_)
{
    //this code limits the sphere from leaving the boundry
    Coord cirPos = createCoord(x_, y_);
    Coord midPoint = createCoord(0, 0);
    Coord vecA = createVector(cirPos, midPoint); // point 1 to center point

    float phi = atan2(y_, x_);
    float magOfVecA = clampIt(magnitude(vecA), 0 , 1);

    x = magOfVecA * cos(phi);
    y = magOfVecA * sin(phi);
}

void Sphere::updatePosition(std::vector<std::unique_ptr<Sphere>>& thing)
{
    for (int i = 0; i < thing.size() -1 ; i++)
    {
        thing[i + 1]->x = (pow(thing[i]->x, 2) + -pow(thing[i]->y, 2)) +
        (pow(0, 2) + -pow(0, 2));
        thing[i + 1]->y = 2 * thing[i]->x * thing[i]->y + 2 * 0 * 0;
    }
}

bool Sphere::checkIntersection(double &rotatingArm, bool other)
{
    Coord cirPos = createCoord(x * 160, y * 160);
    Coord midPoint = createCoord(0, 0);
    Coord rotPoint = createCoord(160 * cos(rotatingArm), 160 * sin(rotatingArm));
    
    Coord vecA = createVector(cirPos, midPoint);
    Coord vecB = createVector(rotPoint, midPoint);
    Coord nVecB = normalise(vecB);
    
    double scalarProjection = clampIt(dotProduct(vecA, nVecB), 0, 160);
    
    spx = scalarProjection * cos(rotatingArm);
    spy = scalarProjection * sin(rotatingArm);
    
    float dist0 = distance(rotPoint.x, rotPoint.y, cirPos.x, cirPos.y);
    float dist1 = distance(cirPos.x, cirPos.y, spx, spy);
    float dist2 = distance(cirPos.x, cirPos.y, midPoint.x, midPoint.y);

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


void Sphere::paint (juce::Graphics& g)
{
    g.setOrigin(getWidth() / 2 - 10, getHeight() / 2 - 10 + 20);
    g.setColour(juce::Colour::fromFloatRGBA (1.0f, 0.0f, 0.0f, 0.5f));
    g.fillEllipse(x * 160, y * 160, sphereRadius * 2, sphereRadius * 2);
}


void Sphere::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

