/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Sphere.h"
#include "myFunctions.h"
#include "MyBtn.h"

//structs that contain atomics so data can be passed from the audio thread to the GUI thread reliably as they both run at different speeds.
//AtomicRotation passes the rotation position in radians the the GUI thread
struct AtomicRotation
{
    AtomicRotation(std::atomic <double>& valueToUse) : value(valueToUse) {}
    //this is getting called 60 times a second in timer callback in pluginEditor.cpp
    double getValue()
    {
        return value.load();
    }
    std::atomic <double>& value;
};

//AtomicRotation passes the DAWs BPM to the the GUI thread
struct AtomicBPM
{
    AtomicBPM(std::atomic <int>& bpmToUse) : bpm(bpmToUse) {}
    
    int getBPM()
    {
        return bpm.load();
    }
    std::atomic <int>& bpm;
};

//AtomicBtn passes the DAWs isPlaying bool to the the GUI thread
struct AtomicBtn
{
    AtomicBtn(std::atomic <bool>& btnStateToUse) : btnState(btnStateToUse) {}
    
    bool getBtnState()
    {
        return btnState.load();
    }
    std::atomic <bool>& btnState;
};


//==============================================================================
/**
*/
//inherit all the necessary classes for the pluginEditor
class Mandelbrot_pluginAudioProcessorEditor  : public juce::AudioProcessorEditor
                                              ,public juce::Timer
                                              ,public juce::Slider::Listener
                                              ,public juce::ComboBox::Listener
                                              ,public juce::Button::Listener
                                              
{
public:
    Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor&);
    ~Mandelbrot_pluginAudioProcessorEditor() override;
    
    //variables for keeping track of the border and node circles radius
    int borderRadius = 140;
    int sphereRad = 7;
    
    int note;
    int octave;
    int scale;
    int rootNote;
    int midiNote;
    
    //int btnCount { 0 };
    
    juce::Image sliderImage;
    juce::Image sinImage;
    juce::Image cosImage;
    juce::Image noiseImage;
    
    float xPos { 0 };
    float yPos { 0 };
    float cxPos { 0 };
    float cyPos { 0 };
    
    float xFreq { 0 };
    float yFreq { 0 };
    float cxFreq { 0 };
    float cyFreq { 0 };
    
    //string to represent mode of position modulation sections
    std::string xMode;
    std::string yMode;
    std::string cxMode;
    std::string cyMode;
    
    /*
     nested vector containing musical scales
     [0] = major scale 1 oct,
     [1] = major scale 2 oct,
     [2] = minor scale 1 oct,
     [3] = minor scale 2 oct,
     [4] = major 7 arp 3 oct,
     [5] = minor 7 arp 3 oct
     */
    const std::vector<std::vector<int> > editorScalesVector{{0, 2, 4, 5, 7, 9, 11, 12},
                                                    {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24},
                                                    {0, 2, 3, 5, 7, 8, 10, 12},
                                                    {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24},
                                                    {0, 4, 7, 11, 12, 16, 19, 23, 24, 28, 31, 35},
                                                    {0, 3, 7, 10, 12, 15, 19, 22, 24, 27, 31, 34}};
    
    //instances of atomics (written at the top of this file)
    AtomicRotation rotationValue;
    AtomicBPM bpmFromAudioThread;
    AtomicBtn btnBoolFromAudioThread;
    
    //used to automate the item of the note selection combobox from inside the DAW
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboAttach;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox*) override;
    void buttonClicked(juce::Button* button) override;
    void updateComboBoxes();

private:
    
    //sliders
    juce::Slider xPos_Slider;
    juce::Slider yPos_Slider;
    juce::Slider BPM_Slider;
    juce::Slider constXOffSet;
    juce::Slider constYOffSet;
    juce::Slider noteAmount;
    juce::Slider noteDuration_slider;
    
    //Comboboxes
    juce::ComboBox noteSelection;
    juce::ComboBox octaveSelection;
    juce::ComboBox scaleSelection;
    juce::ComboBox midiChan;
    
    //enum class used to declare if a button stat is on or off, used in the toggling of the rotation speed buttons buttons
    enum class OnState {
        on,
        off
    };
    
    OnState synchBtnOnState { OnState::off };
    OnState doubleSpeedBtnOnState { OnState::off };
    OnState normalSpeedBtnOnState { OnState::on };
    OnState halfSpeedBtnOnState { OnState::off };
    
    //button counters these are used to detect if a button is on / off used for the velocity & play/stop button
    int velBtnCount { 0 };
    int playStopBtnCount { 0 };
    
    //text buttons
    juce::TextButton synchBtn { "Synch" };
    juce::TextButton doubleSpeedBtn { "* 2" };
    juce::TextButton normalSpeedBtn { "* 1" };
    juce::TextButton halfSpeedBtn { "/ 2" };
    juce::TextButton velBtn { "Velocity" };
    
   
    
    //MyBtn instances
    MyBtn myBtn_one;
    MyBtn myBtn_two;
    MyBtn myBtn_three;
    MyBtn myBtn_four;
    
    //declaring lamda functions for is clicked behaviour on MyBtn instances
    void btnOneIsClicked();
    void btnTwoIsClicked();
    void btnThreeIsClicked();
    void btnFourIsClicked();
    
    //images containing play and stop icons for play/stop button
    juce::Image playBtnImage;
    juce::Image stopBtnImage;
    juce::ImageComponent playStopBtnImageComp;
    juce::TextButton playStopBtn;
    
    //boolean values for keeping track of whats true or not
    bool isPlaying { false };
    bool synchBool { false };
    
    //set rotation so the rotating arm is pointing directly upwards
    double rotation { -M_PI / 2 };

    //vector to store instances of node circle class
    std::vector<std::unique_ptr<Sphere>> vectorOfSpheres;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Mandelbrot_pluginAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mandelbrot_pluginAudioProcessorEditor)
};


