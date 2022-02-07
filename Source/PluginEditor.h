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
                                              ,public juce::ComboBox::Listener
{
public:
    Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor&);
    ~Mandelbrot_pluginAudioProcessorEditor() override;
    int borderRadius = 180;
    float t { 0.0 };
    
    /*
     nested vector containing scales for later
     scalesArray[0] = major scale 1 oct,
     scalesArray[1] = major scale 2 oct,
     scalesArray[2] = minor scale 1 oct,
     scalesArray[3] = minor scale 2 oct
     */
    const std::vector<std::vector<int> > scalesArray{{0, 2, 4, 5, 7, 9, 11, 12},
                                                    {0, 2, 4, 5, 7, 9, 11, 12,14, 16, 17, 19, 21, 23, 24},
                                                    {0, 2, 3, 5, 7, 8, 10, 12},
                                                    {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24}};
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox*) override;

private:
    
    juce::Slider xPos_Slider;
    juce::Slider yPos_Slider;
    juce::ComboBox noteSelection;
    juce::ComboBox octaveSelection;
    juce::ComboBox scaleSelection;
    Spheres sphere;
    std::vector<std::unique_ptr<Spheres>> sphereArray;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Mandelbrot_pluginAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mandelbrot_pluginAudioProcessorEditor)
};
