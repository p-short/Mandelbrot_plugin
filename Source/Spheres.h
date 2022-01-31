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
    
//    std::vector<Spheres> instanceArray;
    
    Spheres();
    ~Spheres() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    // variables
    float xSliderVals {0.0};
    float ySliderVals {0.0};
    int sphereRadius { 10 };
    
private:
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spheres)
};
