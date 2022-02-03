/*
  ==============================================================================

    Spheres.cpp
    Created: 29 Jan 2022 3:24:01pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Spheres.h"
#include "myFunctions.h"

//==============================================================================
Spheres::Spheres()
{

}

Spheres::~Spheres()
{
}

void Spheres::setPosition(float x_, float y_)
{
    Coord cirPos = createCoord(x_ * 170, y_ * 170);
    Coord midPoint = createCoord(0.0, 0.0);
    Coord vecA = createVector(cirPos, midPoint); // point 1 to center point
    
    float phi = atan2(y_, x_);
    float magOfVecA = clampIt(magnitude(vecA), 0 , 170);
    
//    std::cout << phi << std::endl;
    
    x = magOfVecA * cos(phi);
    y = magOfVecA * sin(phi);
}



void Spheres::paint (juce::Graphics& g)
{

    g.setOrigin(getWidth() / 2 - 10, getHeight() / 2 - 10);
    g.setColour(juce::Colours::red);
    g.drawEllipse(x, -y, sphereRadius * 2, sphereRadius * 2, 2);
}



void Spheres::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

