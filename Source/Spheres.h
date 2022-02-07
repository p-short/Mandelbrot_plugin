/*
  ==============================================================================

    Spheres.h
    Created: 29 Jan 2022 3:24:01pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Spheres  : public juce::Component
{
public:
    Spheres(); //float _x, float _y
    ~Spheres() override;
    
    void setPosition(float x_, float y_);
    void updatePosition();
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    float x;
    float y;
    int sphereRadius { 10 };
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spheres)
};



