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


struct AtomicRotation
{
    AtomicRotation(std::atomic <double>& valueToUse) : value(valueToUse) {}
    //this is getting called 60 times a second in timer callback
    double getValue()
    {
        return value.load();
    }
    std::atomic <double>& value;
};


//==============================================================================
/**
*/
class Mandelbrot_pluginAudioProcessorEditor  : public juce::AudioProcessorEditor
                                              ,public juce::Timer
                                              ,public juce::Slider::Listener
                                              ,public juce::ComboBox::Listener
                                              ,public juce::Button::Listener
                                              
{
public:
    Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor&);
    ~Mandelbrot_pluginAudioProcessorEditor() override;
    int borderRadius = 170;
    double t { -M_PI / 2 };
//    double speed { 0.0 };
    float divBy { 1.0 };
//    float fullRotation;
//    int synchBtnCount { 0 };
    
    int note;
    int octave;
    int scale;
    int rootNote;
    int midiNote;
    int btnCount { 0 };
    
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
    
    std::string xMode;
    std::string yMode;
    std::string cxMode;
    std::string cyMode;
    
    /*
     nested vector containing scales for later
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
    
    AtomicRotation rotationValue;
    
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox*) override;
    void buttonClicked(juce::Button* button) override;
    void updateComboBoxes();

private:
    
    juce::Slider xPos_Slider;
    juce::Slider yPos_Slider;
    juce::Slider BPM_Slider;
    juce::Slider constXOffSet;
    juce::Slider constYOffSet;
    juce::Slider noteAmount;
    juce::Slider noteDuration_slider;
    
    juce::ComboBox noteSelection;
    juce::ComboBox octaveSelection;
    juce::ComboBox scaleSelection;
    juce::ComboBox midiChan;
    
    enum class OnState {
        on,
        off
    };
    
    OnState synchBtnOnState { OnState::off };
    OnState doubleSpeedBtnOnState { OnState::off };
    OnState normalSpeedBtnOnState { OnState::on };
    OnState halfSpeedBtnOnState { OnState::off };
    int velBtnCount { 0 };
    
    juce::TextButton synchBtn { "Synch" };
    juce::TextButton doubleSpeedBtn { "* 2" };
    juce::TextButton normalSpeedBtn { "* 1" };
    juce::TextButton halfSpeedBtn { "/ 2" };
    juce::TextButton velBtn { "Velocity" };
    
    juce::ImageComponent btnImageComp;
    juce::TextButton xModBtn;
    juce::Slider xModSpeedSlider;
    juce::Slider xModAmpSlider;
    
    MyBtn myBtn_one;
    MyBtn myBtn_two;
    MyBtn myBtn_three;
    MyBtn myBtn_four;
    
    void btnOneIsClicked();
    void btnTwoIsClicked();
    void btnThreeIsClicked();
    void btnFourIsClicked();
    
    juce::Image playBtnImage;
    juce::Image stopBtnImage;
    juce::ImageComponent playStopBtnImageComp;
    juce::TextButton playStopBtn;
    int playStopBtnCount { 0 };
    bool isPlaying { true };
    
    double rotation { -M_PI / 2 };

    std::vector<std::unique_ptr<Sphere>> vectorOfSpheres;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Mandelbrot_pluginAudioProcessor& audioProcessor;
    
public:
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboAttach;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mandelbrot_pluginAudioProcessorEditor)
};


