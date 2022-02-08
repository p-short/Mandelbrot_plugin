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
class Sphere  : public juce::Component
{
public:
    Sphere(); //float _x, float _y
    ~Sphere() override;
    

    void setPosition(float x_, float y_);
    void updatePosition(std::vector<std::unique_ptr<Sphere>>&);
    void paint (juce::Graphics&) override;
    void resized() override;
    
    float x;
    float y;
    
private:
    
  
    int sphereRadius { 10 };
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sphere)
};



