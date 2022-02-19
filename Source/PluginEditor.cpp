/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Sphere.h"


//==============================================================================
Mandelbrot_pluginAudioProcessorEditor::Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    for (auto i = 0; i < 5; i++)
    {
        vectorOfSpheres.push_back(std::make_unique<Sphere>());
        addAndMakeVisible(*vectorOfSpheres.back());
    }
    
    //xpos slider
    xPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    xPos_Slider.setRange(-1.0f, 1.0f, 0.001f);
    xPos_Slider.setValue(0.0f);
    xPos_Slider.addListener(this);
    addAndMakeVisible(xPos_Slider);
    
    //ypos slider
    yPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    yPos_Slider.setRange(-1.0f, 1.0f, 0.001f);
    yPos_Slider.setValue(0.0f);
    yPos_Slider.addListener(this);
    addAndMakeVisible(yPos_Slider);
    
    //const x offset slider
    constXOffSet.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    constXOffSet.setRange(-1.0f, 1.0f, 0.001f);
    constXOffSet.setValue(0.0f);
    constXOffSet.addListener(this);
    addAndMakeVisible(constXOffSet);
    
    //const y offset slider
    constYOffSet.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    constYOffSet.setRange(-1.0f, 1.0f, 0.001f);
    constYOffSet.setValue(0.0f);
    constYOffSet.addListener(this);
    addAndMakeVisible(constYOffSet);
    
    //BMP slider
    BPM_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    BPM_Slider.setTextValueSuffix(" BPM");
    BPM_Slider.setRange(0, 300, 1);
    BPM_Slider.setValue(120);
    BPM_Slider.addListener(this);
    addAndMakeVisible(BPM_Slider);
    
    //note selection combobox
    noteSelection.addItem("C", 1);
    noteSelection.addItem("C#", 2);
    noteSelection.addItem("D", 3);
    noteSelection.addItem("D#", 4);
    noteSelection.addItem("E", 5);
    noteSelection.addItem("F", 6);
    noteSelection.addItem("F#", 7);
    noteSelection.addItem("G", 8);
    noteSelection.addItem("G#", 9);
    noteSelection.addItem("A", 10);
    noteSelection.addItem("A#", 11);
    noteSelection.addItem("B", 12);
    noteSelection.setJustificationType(juce::Justification::centred);
    noteSelection.addListener(this);
    noteSelection.setSelectedId(1);
    addAndMakeVisible(noteSelection);
    
    //octave selection combobox
    octaveSelection.addItem("0", 1);
    octaveSelection.addItem("1", 2);
    octaveSelection.addItem("2", 3);
    octaveSelection.addItem("3", 4);
    octaveSelection.addItem("4", 5);
    octaveSelection.addItem("5", 6);
    octaveSelection.addItem("6", 7);
    octaveSelection.addItem("7", 8);
    octaveSelection.addItem("8", 9);
    octaveSelection.addItem("9", 10);
    octaveSelection.addItem("10", 11);
    octaveSelection.setJustificationType(juce::Justification::centred);
    octaveSelection.addListener(this);
    octaveSelection.setSelectedId(5);
    addAndMakeVisible(octaveSelection);
    
    //scale selection combobox
    
    scaleSelection.addItem("Major 1 Oct", 1);
    scaleSelection.addItem("Major 2 Oct", 2);
    scaleSelection.addItem("Minor 1 Oct", 3);
    scaleSelection.addItem("Minor 2 Oct", 4);
    scaleSelection.setJustificationType(juce::Justification::centred);
    scaleSelection.addListener(this);
    scaleSelection.setSelectedId(1);
    addAndMakeVisible(scaleSelection);
    
    //synch button
    synchBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
    synchBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(synchBtn);
    synchBtn.addListener(this);
    
    //double speed button
    doubleSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
    doubleSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(doubleSpeedBtn);
    doubleSpeedBtn.addListener(this);
    
    //normal speed button
    normalSpeedBtn.setToggleState(true, juce::NotificationType::dontSendNotification);
    normalSpeedBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgreen);
    addAndMakeVisible(normalSpeedBtn);
    normalSpeedBtn.addListener(this);
    
    //half speed button
    halfSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
    halfSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(halfSpeedBtn);
    halfSpeedBtn.addListener(this);
    
    //image button
    auto sliderImage = juce::ImageCache::getFromMemory(BinaryData::Slider_png, BinaryData::Slider_pngSize);
    
    // start timer to create a loop for animation
    Timer::startTimer(60);
    setSize (600, 400);
}

Mandelbrot_pluginAudioProcessorEditor::~Mandelbrot_pluginAudioProcessorEditor()
{
    Timer::stopTimer();
}

//==============================================================================
void Mandelbrot_pluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // drawing big circle with rotating arm
    g.fillAll (juce::Colours::dimgrey);
    g.setColour(juce::Colours::black);
    g.drawEllipse(300 - borderRadius, 220 - borderRadius, borderRadius * 2, borderRadius * 2, 2);
    g.setOrigin(getWidth() / 2, getHeight() / 2 + 20);
    g.drawLine(0, 0, borderRadius * cos(t), borderRadius * sin(t), 2);
    
    for (auto i = 0; i < vectorOfSpheres.size() -1; i++)
    {
        g.drawLine(vectorOfSpheres[i]->getXPos() * 160, vectorOfSpheres[i]->getYPos() * 160,
                   vectorOfSpheres[i +1]->getXPos() * 160, vectorOfSpheres[i +1]->getYPos() * 160, 1.5);
    }
    
//    g.setColour(juce::Colours::green);
//
//    for (auto i = 0; i < vectorOfSpheres.size(); i++)
//    {
//        g.drawLine(vectorOfSpheres[i]->getXPos() * 160, vectorOfSpheres[i]->getYPos() * 160,
//                   vectorOfSpheres[i]->spx, vectorOfSpheres[i]->spy, 3);
//
//    }
    
}


void Mandelbrot_pluginAudioProcessorEditor::timerCallback()
{
    repaint();
   
    for (int i = 0; i < vectorOfSpheres.size(); i++)
    {
        midiNote = rootNote + scalesVector[scale][i];
        vectorOfSpheres[i]->setPosition(xPos_Slider.getValue(), yPos_Slider.getValue(),
                                        constXOffSet.getValue(), constYOffSet.getValue());
        vectorOfSpheres[i]->updatePosition(vectorOfSpheres);
        vectorOfSpheres[i]->limitSphere();
        
        if (vectorOfSpheres[i]->checkIntersection(t, vectorOfSpheres[i]->getSphereBool()))
        {
            
//                std::cout << "shpere " << i + 1 << " boom" << std::endl;
                //std::cout << midiNote << std::endl;
                vectorOfSpheres[i]->setSphereBool(false);
        }
    }
    
    //update speed acording to speed buttons
    t += M_PI * 2 / (60 / (BPM_Slider.getValue() / 4) / divBy * 60);;
}

// listen for slider value changes, pass them to variables in Sphere instance
void Mandelbrot_pluginAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == & xPos_Slider)
    {
        //sphere.x = xPos_Slider.getValue();
    }
    else if (slider == & yPos_Slider)
    {
        //sphere.y = yPos_Slider.getValue();
    }
    else if (slider == & BPM_Slider)
    {
//        speed = fullRotation;
    }
}

void Mandelbrot_pluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    if (box == &noteSelection)
    {
        note = noteSelection.getSelectedId() - 1;
        updateComboBoxes();
    }
    
    if (box == &octaveSelection)
    {
        octave = (octaveSelection.getSelectedId() - 1) * 12;
        updateComboBoxes();
    }
    
    if (box == &scaleSelection)
    {
        scale = scaleSelection.getSelectedId() - 1;
        updateComboBoxes();
    }
    
}

void Mandelbrot_pluginAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &synchBtn)
    {
        //eventully add the code to give plug acsess to Abeltons tempo.
        if (synchBtnOnState == OnState::off)
        {
            synchBtnOnState = OnState::on;
            synchBtn.setToggleState(true, juce::NotificationType::dontSendNotification);
            synchBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgreen);
        }
        
        else if (synchBtnOnState == OnState::on)
        {
            synchBtnOnState = OnState::off;
            synchBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
            synchBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        }
    }
    
    if (button == &doubleSpeedBtn)
    {
        divBy = 2;
        
        normalSpeedBtnOnState = OnState::off;
        normalSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
        normalSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        
        halfSpeedBtnOnState = OnState::off;
        halfSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
        halfSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        
        if (doubleSpeedBtnOnState == OnState::off)
        {
            doubleSpeedBtnOnState = OnState::on;
            doubleSpeedBtn.setToggleState(true, juce::NotificationType::dontSendNotification);
            doubleSpeedBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgreen);
        }
    }
    
    if (button == &normalSpeedBtn)
    {
        divBy = 1;
        
        doubleSpeedBtnOnState = OnState::off;
        doubleSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
        doubleSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        
        halfSpeedBtnOnState = OnState::off;
        halfSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
        halfSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        
        if (normalSpeedBtnOnState == OnState::off)
        {
            normalSpeedBtnOnState = OnState::on;
            normalSpeedBtn.setToggleState(true, juce::NotificationType::dontSendNotification);
            normalSpeedBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgreen);
        }
    }
    
    if (button == &halfSpeedBtn)
    {
        divBy = 0.5;
        
        doubleSpeedBtnOnState = OnState::off;
        doubleSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
        doubleSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        
        normalSpeedBtnOnState = OnState::off;
        normalSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
        normalSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        
        if (halfSpeedBtnOnState == OnState::off)
        {
            halfSpeedBtnOnState = OnState::on;
            halfSpeedBtn.setToggleState(true, juce::NotificationType::dontSendNotification);
            halfSpeedBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgreen);
        }
    }
}

void Mandelbrot_pluginAudioProcessorEditor::updateComboBoxes()
{
    rootNote = note + octave;
}

void Mandelbrot_pluginAudioProcessorEditor::resized()
{
    // set positions
    
    //dropdown boxes
    int spacing = 5;
    noteSelection.setBounds(spacing, 10, 60, 20);
    octaveSelection.setBounds(60 + spacing, 10, 60, 20);
    scaleSelection.setBounds(120 + spacing, 10, 100, 20);
    
    //speed slider
    BPM_Slider.setBounds(360, 10, 240, 20);
    
    //synch button
    synchBtn.setBounds(360, 30, 50, 20);
    
    //double speed button
    doubleSpeedBtn.setBounds(410, 30, 50, 20);
    
    //normal speed button
    normalSpeedBtn.setBounds(460, 30, 50, 20);
    
    //half speed button
    halfSpeedBtn.setBounds(510, 30, 50, 20);
    
    //image button
//    myImageBtn.setBounds(100, 350, 50, 30);
    
    //Sphere instances
    for (auto& sphere : vectorOfSpheres)
    {
        sphere->setBounds (getLocalBounds());
    }
    
    //Sphere position sliders
    xPos_Slider.setBounds(22.5, getHeight() / 2 - 100, 20, 200);
    yPos_Slider.setBounds(87.5, getHeight() / 2 - 100, 20, 200);
    
    constXOffSet.setBounds(492.5, getHeight() / 2 - 100, 20, 200);
    constYOffSet.setBounds(555.5, getHeight() / 2 - 100, 20, 200);
}
