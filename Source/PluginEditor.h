/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Spheres.h"

//==============================================================================
/**
*/
class Mandelbrot_pluginAudioProcessorEditor  : public juce::AudioProcessorEditor
                                              ,public juce::Timer
                                              ,public juce::Slider::Listener
{
public:
    Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor&);
    ~Mandelbrot_pluginAudioProcessorEditor() override;
    int borderRadius = 180;
    float t { 0.0 };

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    
    juce::Slider xPos_Slider;
    juce::Slider yPos_Slider;
    juce::ComboBox noteSelection;
    juce::ComboBox octaveSelection;
    juce::ComboBox scaleSelection;
    Spheres sphere;
    std::vector<Spheres> myArray;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Mandelbrot_pluginAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mandelbrot_pluginAudioProcessorEditor)
};
