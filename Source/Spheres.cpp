/*
  ==============================================================================

    Spheres.cpp
    Created: 29 Jan 2022 3:24:01pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Spheres.h"

//==============================================================================
Spheres::Spheres()
{
}

Spheres::~Spheres()
{
}

void Spheres::paint (juce::Graphics& g)
{

    g.setOrigin(getWidth() / 2 - 10, getHeight() / 2 - 10);
    g.setColour(juce::Colours::red);
    
    
    g.drawEllipse((xSliderVals * 170), (ySliderVals * 170),
                  sphereRadius * 2, sphereRadius * 2, 2);
}



void Spheres::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
