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
class MyBtn  : public juce::Component,
public juce::Button::Listener,
public juce::Slider::Listener

{
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
    juce::TextButton btn;
    
private:
    
    int btnCount { 0 };
    juce::Image sliderImage;
    juce::Image sinImage;
    juce::Image cosImage;
    juce::Image noiseImage;
    juce::ImageComponent myBtnImage;
    juce::Slider speedSlider;
    juce::Slider ampSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyBtn)
};

