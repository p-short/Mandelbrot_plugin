/*
  ==============================================================================

    MyBtn.h
    Created: 22 Feb 2022 10:33:36am
    Author:  Iansyst Loan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */

//myBtn class inherits properties from JUCE component and listener classes
class MyBtn  : public juce::Component,
public juce::Button::Listener,
public juce::Slider::Listener

{
    //declarations of methods used within myBtn class
public:
    MyBtn();
    ~MyBtn() override;
    
    void paint (juce::Graphics&) override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void resized() override;
    int getBtnCount();
    float getSpeedSliderVal();
    float getAmpSliderVal();
    void setModName(std::string _modName, float _textOffset);
    void disableSliders(std::string _stringToCheck);
    juce::TextButton btn;
    
private:
    
    //private member variables of myBtn hold all the nesserasy GUI components and images to use.
    int btnCount { 0 };
    juce::Image sliderImage;
    juce::Image sinImage;
    juce::Image cosImage;
    juce::Image noiseImage;
    juce::ImageComponent myBtnImage;
    juce::Slider speedSlider;
    juce::Slider ampSlider;
    std::string modName;
    float textOffset;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyBtn)
};

