/*
  ==============================================================================

    MyBtn.cpp
    Created: 22 Feb 2022 10:33:36am
    Author:  Iansyst Loan

  ==============================================================================
*/


#include <JuceHeader.h>
#include "MyBtn.h"

//myBtn class encapsulates multiple buttons and sliders used for the position modulation section along the bottom of the UI. multiple instances of myBtn are created within PluginEditor

//==============================================================================
MyBtn::MyBtn()
{
    //Images for buttons
    //the images are all png files containing relevant graphics with a transparent background so they can be laid on top of GUI
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
    
    //set overall look and feel of MyBtn sliders
    getLookAndFeel().setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    getLookAndFeel().setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    getLookAndFeel().setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    
    //set up speed slider. this controls the frequency of the chosen modulation source
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    speedSlider.setRange(0.0, 0.2, 0.001);
    speedSlider.setValue(0.0);
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    
    //set up amp slider. this controls the amplitude of the chosen modulation source
    ampSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    ampSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    ampSlider.setRange(0.0, 1.0, 0.001);
    ampSlider.setValue(0.0);
    addAndMakeVisible(ampSlider);
    ampSlider.addListener(this);
}

MyBtn::~MyBtn()
{
}

//draw labels to MyBtn
void MyBtn::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.setFont(12);
    //"modName" in main title and text offset of MyBtn is passed in and initialised in the setModName method.
    g.drawText(modName + " Pos Mod", textOffset, 0, 100, 20, juce::Justification::horizontallyJustified);
    g.setColour(juce::Colours::black);
    g.drawText("Speed", 17, 15, 100, 20, juce::Justification::horizontallyJustified);
    g.drawText("Scale", 19, 39, 100, 20, juce::Justification::horizontallyJustified);
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

//pure vitual function needs to be included
void MyBtn::sliderValueChanged(juce::Slider* slider)
{
}

//set size and position of the components of myBtn
void MyBtn::resized()
{
    speedSlider.setBounds(0, 27, 65, 20);
    ampSlider.setBounds(0, 51, 65, 20);
    myBtnImage.setBounds(7, 72, 50, 40);
    btn.setBounds(7, 72, 50, 40);
}

// return the btn count. This is used to return what state myBtn is in. This is used to change modulation source within the pluginEditor
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

//arguments pass set the string and text offset used in the paint method
void MyBtn::setModName(std::string _modName, float _textOffset)
{
    modName = _modName;
    textOffset = _textOffset;
}

// when myBtn is on slider mode remove some of the alpha from the sliders appearance and disable modulation section sliders so they cant be moved. When in slider mode only the sliders can move the position of the node circles.
void MyBtn::disableSliders(std::string _stringToCheck)
{
    if (_stringToCheck == "slider")
    {
        speedSlider.setValue(0);
        speedSlider.setEnabled(false);
        speedSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126).withAlpha(0.5f));
        speedSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68).withAlpha(0.5f));
        speedSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68).withAlpha(0.5f));
        
        ampSlider.setValue(0);
        ampSlider.setEnabled(false);
        ampSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126).withAlpha(0.5f));
        ampSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68).withAlpha(0.5f));
        ampSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68).withAlpha(0.5f));
    }
    else
    {
        speedSlider.setEnabled(true);
        speedSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
        speedSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
        speedSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
        
        ampSlider.setEnabled(true);
        ampSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
        ampSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
        ampSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    }
}
