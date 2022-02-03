/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Spheres.h"

//==============================================================================
Mandelbrot_pluginAudioProcessorEditor::Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // I would like to push_back an Instance of the Sphere class in to a vector and read from it but am not sure in what .cpp file i should do this. Im pretty muddled up with the scope of everything TBH.
    
    setSize (600, 400);
    // add Sphere instance
    addAndMakeVisible(sphere);
    

    //add sliders, set range, value ect..
    xPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    xPos_Slider.setRange(-1.0f, 1.0f, 0.01f);
    xPos_Slider.setValue(0.0f);
    xPos_Slider.addListener(this);
    addAndMakeVisible(xPos_Slider);
    
    yPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    yPos_Slider.setRange(-1.0f, 1.0f, 0.001f);
    yPos_Slider.setValue(0.0f);
    yPos_Slider.addListener(this);
    addAndMakeVisible(yPos_Slider);
    
    // start timer to create a loop for animation
    Timer::startTimer(60);
}

Mandelbrot_pluginAudioProcessorEditor::~Mandelbrot_pluginAudioProcessorEditor()
{
    Timer::stopTimer();
}

//==============================================================================
void Mandelbrot_pluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // drawing big circle with rotating arm
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setOrigin(getWidth() / 2, getHeight() / 2);
    g.drawEllipse(0 - borderRadius, 0 - borderRadius, borderRadius * 2, borderRadius * 2, 2);
    g.drawLine(0, 0, borderRadius * cos(t), borderRadius * sin(t), 2);
}

// calling repaint 60 time a second on *this* and  Sphere instance
void Mandelbrot_pluginAudioProcessorEditor::timerCallback()
{
    repaint();
    sphere.repaint();
    t += 0.01;
    sphere.setPosition(xPos_Slider.getValue(), yPos_Slider.getValue());
}

// listen for slider value changes, pass them to variables in Sphere instance
void Mandelbrot_pluginAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == & xPos_Slider)
    {
        //sphere.xSliderVals = xPos_Slider.getValue();
    }
    else if (slider == & yPos_Slider)
    {
        //sphere.ySliderVals = yPos_Slider.getValue();
    }
}

void Mandelbrot_pluginAudioProcessorEditor::resized()
{
    // set positions
    sphere.setBounds(getLocalBounds());
    xPos_Slider.setBounds(getWidth() / 12, getHeight() / 2 - 100, 20, 200);
    yPos_Slider.setBounds(getWidth() / 12 + 50, getHeight() / 2 - 100, 20, 200);
}
