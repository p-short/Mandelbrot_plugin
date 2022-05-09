/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "sphereLogic.h"
#include "myFunctions.h"
#include "NoteOff.h"

#define KEY_ID "key"
#define KEY_NAME "Key"

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
    
    std::atomic <double> currentInfo { 0 };
    std::atomic <int> currentBPM { 120 };
    std::atomic <bool> currentBtnState;
    double tp { M_PI * 2 };
    double startAng { -M_PI / 2 };

    
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
    int apMidiChan { 1 };
    bool apIsVel { false };
    bool apIsPlaying { false };
    bool playHeadIsPlaying;
    
    int apNoteAmount { 7 };
    int apNoteDuration { 3 };
    int apPlaybackSpeed { 64 };
    bool apSynch { false };
    float apBPM { 120 };
    float tempBPM { 120 };
    bool apisPlayBtn { false };
    
    /*
     nested vector containing scales for later
     [0] = major scale 1 oct,
     [1] = major scale 2 oct,
     [2] = minor scale 1 oct,
     [3] = minor scale 2 oct,
     [4] = major 7 arp 3 oct,
     [5] = minor 7 arp 3 oct
     */
    const std::vector <std::vector<int>> scalesVector{{0, 2, 4, 5, 7, 9, 11, 12},
        {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24},
        {0, 2, 3, 5, 7, 8, 10, 12},
        {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24},
        {0, 4, 7, 11, 12, 16, 19, 23, 24, 28, 31, 35},
        {0, 3, 7, 10, 12, 15, 19, 22, 24, 27, 31, 34}};

    const juce::StringArray keyIdArray{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    
        
    juce::AudioProcessorValueTreeState tree;

private:
//    juce::MidiBuffer processedBuffer;
    std::vector <std::unique_ptr <SphereLogic>> sphereLogicVector;
    
    std::vector <std::unique_ptr <NoteOff>> noteOffVector;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mandelbrot_pluginAudioProcessor)
};
