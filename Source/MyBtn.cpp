/*
  ==============================================================================

    MyBtn.cpp
    Created: 22 Feb 2022 10:33:36am
    Author:  Iansyst Loan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MyBtn.h"


//==============================================================================
MyBtn::MyBtn()
{
    //Images for buttons
    sliderImage = juce::ImageCache::getFromMemory(BinaryData::Slider_png, BinaryData::Slider_pngSize);
    sinImage = juce::ImageCache::getFromMemory(BinaryData::Sine_Wave_png, BinaryData::Sine_Wave_pngSize);
    cosImage = juce::ImageCache::getFromMemory(BinaryData::Cosine_Wave_png, BinaryData::Cosine_Wave_pngSize);
    noiseImage = juce::ImageCache::getFromMemory(BinaryData::White_noise_png, BinaryData::White_noise_pngSize);
    
    //make images visable
    myBtnImage.setImage(sliderImage);
    addAndMakeVisible(myBtnImage);
    
    //make the button transparent so you can see the image underneath
    btn.setColour(juce::TextButton::ColourIds::buttonColourId,
                  juce::Colour::fromFloatRGBA (1.0f, 1.0f, 1.0f, 0.0f));
    addAndMakeVisible(btn);
    btn.addListener(this);
    
    //set up speed slider
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    speedSlider.setRange(0.0, 0.2, 0.001);
    speedSlider.setValue(0.0);
    speedSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    speedSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    speedSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    
    //set up amp slider
    ampSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    ampSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ampSlider.setRange(0.0, 1.0, 0.001);
    ampSlider.setValue(0.0);
    ampSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    ampSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    ampSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    addAndMakeVisible(ampSlider);
    ampSlider.addListener(this);
}

MyBtn::~MyBtn()
{
}

void MyBtn::paint (juce::Graphics& g)
{
}

//cycles through all the images, one image per click
void MyBtn::buttonClicked(juce::Button* button)
{
    if (button == &btn)
    {
        btnCount++;
        btnCount = btnCount % 4;
        switch(btnCount) {
            case 1:
                //sets image to sinwave
                myBtnImage.setImage(sinImage);
                break;
            case 2:
                //sets image to cosinewave
                myBtnImage.setImage(cosImage);
                break;
            case 3:
                //sets image to white noise
                myBtnImage.setImage(noiseImage);
                break;
            case 0:
                //sets image to slider
                myBtnImage.setImage(sliderImage);
                break;
        }
    }
}

void MyBtn::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &speedSlider)
    {
        
    }
    
    if (slider == &ampSlider)
    {
        
    }
}

//set size of custom btn
void MyBtn::resized()
{
    speedSlider.setBounds(0, 0, 50, 20);
    ampSlider.setBounds(0, 23, 50, 20);
    myBtnImage.setBounds(0, 48, 50, 40);
    btn.setBounds(0, 48, 50, 40);
}

// return the btn count, this will be useful later
int MyBtn::getBtnCount()
{
    return btnCount;
}

//return speed slider values
float MyBtn::getSpeedSliderVal()
{
    return speedSlider.getValue();
}

//return amp slider values
float MyBtn::getAmpSliderVal()
{
    return ampSlider.getValue();
}
