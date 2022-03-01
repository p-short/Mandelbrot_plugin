/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "sphereLogic.h"
#include "myFunctions.h"

//==============================================================================
/**
*/
class Mandelbrot_pluginAudioProcessor  : public juce::AudioProcessor

{
public:
    //==============================================================================
    Mandelbrot_pluginAudioProcessor();
    ~Mandelbrot_pluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
//    void paint (juce::Graphics&) override;
  

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float apx_pos { 0 };
    float apy_pos { 0 };
    float apcx_pos { 0 };
    float apcy_pos { 0 };
    double inc { 0.0 };
    
//    int note;
//    int octave;
    int apScale;
    int apRootNote;
    int midiNote;
    bool apIsVel { false };
    
    /*
     nested vector containing scales for later
     scalesArray[0] = major scale 1 oct,
     scalesArray[1] = major scale 2 oct,
     scalesArray[2] = minor scale 1 oct,
     scalesArray[3] = minor scale 2 oct
     */
    const std::vector <std::vector<int>> scalesVector{{0, 2, 4, 5, 7, 9, 11, 12},
        {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24},
        {0, 2, 3, 5, 7, 8, 10, 12},
        {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24}};

private:
//    juce::MidiBuffer processedBuffer;
    std::vector <std::unique_ptr <SphereLogic>> sphereLogicVector;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mandelbrot_pluginAudioProcessor)
};
