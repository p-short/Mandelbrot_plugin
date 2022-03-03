/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include <iostream>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Sphere.h"
#include "noise.h"


//==============================================================================
Mandelbrot_pluginAudioProcessorEditor::Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    auto editorMaxVectorSize = editorScalesVector[0].size();
    
    for (int i = 0; i < editorScalesVector.size(); i++)
    {
        if (editorScalesVector[i].size() > editorMaxVectorSize)
        {
            editorMaxVectorSize = editorScalesVector[i].size();
        }
    }
    
    for (auto i = 0; i < editorMaxVectorSize; i++)
    {
        vectorOfSpheres.push_back(std::make_unique<Sphere>());
//        addAndMakeVisible(*vectorOfSpheres.back());
    }
        
    //xpos slider
    xPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    xPos_Slider.setRange(-1.0f, 1.0f, 0.001f);
    xPos_Slider.setValue(0.0f);
    xPos_Slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    xPos_Slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    xPos_Slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    xPos_Slider.addListener(this);
    addAndMakeVisible(xPos_Slider);
    
    //ypos slider
    yPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    yPos_Slider.setRange(-1.0f, 1.0f, 0.001f);
    yPos_Slider.setValue(0.0f);
    yPos_Slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    yPos_Slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    yPos_Slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    yPos_Slider.addListener(this);
    addAndMakeVisible(yPos_Slider);
    
    //const x offset slider
    constXOffSet.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    constXOffSet.setRange(-1.0f, 1.0f, 0.001f);
    constXOffSet.setValue(0.0f);
    constXOffSet.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    constXOffSet.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    constXOffSet.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    constXOffSet.addListener(this);
    addAndMakeVisible(constXOffSet);
    
    //const y offset slider
    constYOffSet.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    constYOffSet.setRange(-1.0f, 1.0f, 0.001f);
    constYOffSet.setValue(0.0f);
    constYOffSet.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    constYOffSet.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    constYOffSet.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    constYOffSet.addListener(this);
    addAndMakeVisible(constYOffSet);
    
    //BMP slider
    BPM_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    BPM_Slider.setTextValueSuffix(" BPM");
    BPM_Slider.setRange(0, 300, 1);
    BPM_Slider.setValue(120);
    BPM_Slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    BPM_Slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    BPM_Slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    BPM_Slider.setTextBoxIsEditable(false);
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
    noteSelection.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
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
    octaveSelection.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    octaveSelection.addListener(this);
    octaveSelection.setSelectedId(5);
    addAndMakeVisible(octaveSelection);
    
    //scale selection combobox
    
    scaleSelection.addItem("Major 1 Oct", 1);
    scaleSelection.addItem("Major 2 Oct", 2);
    scaleSelection.addItem("Minor 1 Oct", 3);
    scaleSelection.addItem("Minor 2 Oct", 4);
    scaleSelection.addItem("Major 7 arp 3 Oct", 5);
    scaleSelection.addItem("Minor 7 arp 3 Oct", 6);
    scaleSelection.setJustificationType(juce::Justification::centred);
    scaleSelection.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    scaleSelection.addListener(this);
    scaleSelection.setSelectedId(1);
    addAndMakeVisible(scaleSelection);
    
    // midi channle selection
    juce::StringArray midiChanList("Midi Channel 1", "Midi Channel 2", "Midi Channel 3", "Midi Channel 4",
                                   "Midi Channel 5", "Midi Channel 6", "Midi Channel 7", "Midi Channel 8",
                                   "Midi Channel 9", "Midi Channel 10", "Midi Channel 11", "Midi Channel 12",
                                   "Midi Channel 13", "Midi Channel 14", "Midi Channel 15", "Midi Channel 16");
    midiChan.addItemList(midiChanList, 1);
    midiChan.setJustificationType(juce::Justification::centred);
    midiChan.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    midiChan.addListener(this);
    midiChan.setSelectedId(1);
    addAndMakeVisible(midiChan);
    
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
    normalSpeedBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(36, 44, 68));
    addAndMakeVisible(normalSpeedBtn);
    normalSpeedBtn.addListener(this);
    
    //half speed button
    halfSpeedBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
    halfSpeedBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(halfSpeedBtn);
    halfSpeedBtn.addListener(this);
    
    //velocity button
    velBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
    velBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(velBtn);
    velBtn.addListener(this);
    
    //play / stop button images
    playBtnImage = juce::ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
    stopBtnImage = juce::ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize);
    
    playStopBtnImageComp.setImage(playBtnImage);
    addAndMakeVisible(playStopBtnImageComp);
    
    //play / stop button
    playStopBtn.setColour(juce::TextButton::ColourIds::buttonColourId,
                  juce::Colour::fromFloatRGBA (1.0f, 1.0f, 1.0f, 0.0f));
    addAndMakeVisible(playStopBtn);
    playStopBtn.addListener(this);

    
    //custom buttons
    addAndMakeVisible(myBtn_one);
    addAndMakeVisible(myBtn_two);
    addAndMakeVisible(myBtn_three);
    addAndMakeVisible(myBtn_four);
    
    //lamda functions for button clicked inside MyBtn class.
    myBtn_one.btn.onClick = [this] { btnOneIsClicked(); };
    myBtn_two.btn.onClick = [this] { btnTwoIsClicked(); };
    myBtn_three.btn.onClick = [this] { btnThreeIsClicked(); };
    myBtn_four.btn.onClick = [this] { btnFourIsClicked(); };
    
    //set all mode to slider as default
    xMode = "slider"; yMode = "slider"; cxMode = "slider"; cyMode = "slider";
    
    //start timer to create a loop for animation
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
    g.saveState();
    g.fillAll (juce::Colours::dimgrey);
    g.setColour(juce::Colours::black);
    g.drawEllipse(300 - borderRadius, 220 - borderRadius, borderRadius * 2, borderRadius * 2, 2);
    g.setOrigin(getWidth() / 2, getHeight() / 2 + 20);
    g.drawLine(0, 0, borderRadius * cos(t), borderRadius * sin(t), 1);
    

    
    //scale returns 0, 1, 2, 3
    
//    std::cout << "x : " << vectorOfSpheres[14]->getXPos() << "\n" <<
//    "y : " <<vectorOfSpheres[14]->getYPos() << "\n";
    
    if (scale <= editorScalesVector.size() - 1)
    {
    
        for (auto i = 0; i < editorScalesVector[scale].size(); i++)
        {
            if (vectorOfSpheres[i]->checkForPaint(t) && isPlaying)
            {
                g.setColour(juce::Colour::fromFloatRGBA (0.996f, 0.509f, 0.549f, 1.0f));
            }
            
            else
            {
                g.setColour(juce::Colour::fromFloatRGBA (0.996f, 0.509f, 0.549f, 0.5f));
            }
            
            g.fillEllipse((vectorOfSpheres[i]->getXPos() * 160) - 10, (vectorOfSpheres[i]->getYPos() * 160) - 10,
                          10 * 2, 10 * 2);
            
            //line of scalar projection
//            g.setColour(juce::Colours::green);
//            g.drawLine(vectorOfSpheres[i]->getXPos() * 160, vectorOfSpheres[i]->getYPos() * 160,
//                       vectorOfSpheres[i]->spx, vectorOfSpheres[i]->spy, 3);
        }
        
        for (auto j = 0; j < editorScalesVector[scale].size() - 1; j++)
        {
            g.setColour(juce::Colours::black);
            g.drawLine(vectorOfSpheres[j]->getXPos() * 160, vectorOfSpheres[j]->getYPos() * 160,
                       vectorOfSpheres[j +1]->getXPos() * 160, vectorOfSpheres[j +1]->getYPos() * 160, 0.5);
        }
    }
    
//    std::cout << vectorOfSpheres[13]->getDist() << "\n";
    
    g.restoreState();
    
    //x label
    g.saveState();
    g.setColour(juce::Colours::black);
    g.addTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 4, 216));
    g.drawText("x axis", 4, 216, 100, 50, juce::Justification::topLeft);
    g.restoreState();
    
    //y label
    g.saveState();
    g.setColour(juce::Colours::black);
    g.addTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 109, 216));
    g.drawText("y axis", 109, 216, 100, 50, juce::Justification::topLeft);
    g.restoreState();
    
    //x offset label
    g.saveState();
    g.setColour(juce::Colours::black);
    g.addTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 473, 222));
    g.drawText("x offset", 473, 222, 100, 50, juce::Justification::topLeft);
    g.restoreState();
    
    //y offset label
    g.saveState();
    g.setColour(juce::Colours::black);
    g.addTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 579, 222));
    g.drawText("y offset", 579, 222, 100, 50, juce::Justification::topLeft);
    g.restoreState();
}


void Mandelbrot_pluginAudioProcessorEditor::timerCallback()
{
    repaint();
    
    // remember to send these to audio processer to collision detection matches up.
    
    //xMode code
    if (xMode == "slider")
    {
        xPos = xPos_Slider.getValue();
    }
    
    else if (xMode == "sin")
    {
        xPos = sin(xFreq) * myBtn_one.getAmpSliderVal();
    }
    
    else if (xMode == "cos")
    {
        xPos = cos(xFreq) * myBtn_one.getAmpSliderVal();
    }
    
    else if (xMode == "noise")
    {
        xPos = myPerlinNoise(xFreq) * myBtn_one.getAmpSliderVal();
    }
    
    //yMode code
    if (yMode == "slider")
    {
        yPos = yPos_Slider.getValue();
    }
    
    else if (yMode == "sin")
    {
        yPos = sin(yFreq) * myBtn_two.getAmpSliderVal();
    }
    
    else if (yMode == "cos")
    {
        yPos = cos(yFreq) * myBtn_two.getAmpSliderVal();
    }
    
    else if (yMode == "noise")
    {
        yPos = myPerlinNoise(yFreq) * myBtn_two.getAmpSliderVal();
    }
    
    //cxMode code
    if (cxMode == "slider")
    {
        cxPos = constXOffSet.getValue();
    }
    
    else if (cxMode == "sin")
    {
        cxPos = sin(cxFreq) * myBtn_three.getAmpSliderVal();
    }
    
    else if (cxMode == "cos")
    {
        cxPos = cos(cxFreq) * myBtn_three.getAmpSliderVal();
    }
    
    else if (cxMode == "noise")
    {
        cxPos = myPerlinNoise(cxFreq) * myBtn_three.getAmpSliderVal();
    }
    
    //cyMode code
    if (cyMode == "slider")
    {
        cyPos = constYOffSet.getValue();
    }
    
    else if (cyMode == "sin")
    {
        cyPos = sin(cyFreq) * myBtn_four.getAmpSliderVal();
    }
    
    else if (cyMode == "cos")
    {
        cyPos = cos(cyFreq) * myBtn_four.getAmpSliderVal();
    }
    
    else if (cyMode == "noise")
    {
        cyPos = myPerlinNoise(cyFreq) * myBtn_four.getAmpSliderVal();
    }
    
    //xFreq logic
    
    if (xMode != "slider")
    {
        xFreq += myBtn_one.getSpeedSliderVal();
    }
    
    else
    {
        xFreq = 0;
    }
    
    //yFreq logic
    
    if (yMode != "slider")
    {
        yFreq += myBtn_two.getSpeedSliderVal();
    }
    
    else
    {
        yFreq = 0;
    }
    
    //cxFreq logic
    
    if (cxMode != "slider")
    {
        cxFreq += myBtn_three.getSpeedSliderVal();
    }
    
    else
    {
        cxFreq = 0;
    }
    
    //cyFreq logic
    
    if (cyMode != "slider")
    {
        cyFreq += myBtn_four.getSpeedSliderVal();
    }
    
    else
    {
        cyFreq = 0;
    }
   
    for (int i = 0; i < vectorOfSpheres.size(); i++)
    {
//        midiNote = rootNote + scalesVector[scale][i];
        vectorOfSpheres[i]->setPosition(xPos, yPos, cxPos, cyPos);
        vectorOfSpheres[i]->updatePosition(vectorOfSpheres);
        vectorOfSpheres[i]->limitSphere();
        
//        if (vectorOfSpheres[i]->checkIntersection(t, vectorOfSpheres[i]->getSphereBool()))
//        {
//            
////                std::cout << "shpere " << i + 1 << " boom" << std::endl;
//                //std::cout << midiNote << std::endl;
//                vectorOfSpheres[i]->setSphereBool(false);
//        }
        
    }
    
    //update speed acording to speed buttons
    if (isPlaying)
    {
        t += (M_PI * 2) / (60 / (BPM_Slider.getValue() / 4) / divBy * 60);
        //TWO_PI / (60 / (BPMslider.value() / 4) / divBy * 60)
    }
    
    else
    {
        t = -M_PI / 2;
    }
    
    audioProcessor.inc = t;
    
    audioProcessor.apx_pos = xPos;
    audioProcessor.apy_pos = yPos;
    audioProcessor.apcx_pos = cxPos;
    audioProcessor.apcy_pos = cyPos;
    
//    std::cout << "val of x : " << vectorOfSpheres[14]->getXPos() << "\n";
}

// listen for slider value changes, pass them to variables in Sphere instance
void Mandelbrot_pluginAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    //this will be problematic later WATCH OUT.
    if (slider == & xPos_Slider)
    {
//        audioProcessor.apx_pos = xPos;
    }
    else if (slider == & yPos_Slider)
    {
//        audioProcessor.apy_pos = yPos;
    }
    else if (slider == & BPM_Slider)
    {
        //speed = fullRotation;
    }
    else if (slider == & constXOffSet)
    {
//        audioProcessor.apcx_pos = constXOffSet.getValue();
    }
    else if (slider == & constYOffSet)
    {
//        audioProcessor.apcy_pos = constYOffSet.getValue();
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
        audioProcessor.apScale = scaleSelection.getSelectedId() - 1;
        updateComboBoxes();
    }
    
    if (box == &midiChan)
    {
        audioProcessor.apMidiChan = midiChan.getSelectedId();
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
            synchBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(36, 44, 68));
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
            doubleSpeedBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(36, 44, 68));
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
            normalSpeedBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(36, 44, 68));
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
            halfSpeedBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(36, 44, 68));
        }
    }
    
    if (button == &velBtn)
    {
        velBtnCount++;
        velBtnCount = velBtnCount % 2;
        
        if (velBtnCount == 1)
        {
            audioProcessor.apIsVel = true;
            velBtn.setToggleState(true, juce::NotificationType::dontSendNotification);
            velBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(36, 44, 68));
        }
        
        else if (velBtnCount == 0)
        {
            audioProcessor.apIsVel = false;
            velBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
            velBtn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkgrey);
        }
    }
    
    if (button == &playStopBtn)
    {
        playStopBtnCount++;
        playStopBtnCount = playStopBtnCount % 2;
        
        switch (playStopBtnCount)
        {
        case 1:
            isPlaying = true;
            playStopBtnImageComp.setImage(stopBtnImage);
            break;
        
        case 0:
            isPlaying = false;
            playStopBtnImageComp.setImage(playBtnImage);
            break;
        }
        audioProcessor.apIsPlaying = isPlaying;
    }
}

void Mandelbrot_pluginAudioProcessorEditor::updateComboBoxes()
{
//    rootNote = note + octave;
    audioProcessor.apRootNote = note + octave;
}

void Mandelbrot_pluginAudioProcessorEditor::resized()
{
    // set positions
    
    //dropdown boxes
    int spacing = 5;
    noteSelection.setBounds(spacing, 10, 60, 20);
    octaveSelection.setBounds(60 + spacing, 10, 60, 20);
    scaleSelection.setBounds(120 + spacing, 10, 100, 20);
    midiChan.setBounds(spacing, 40, 120, 20);
    
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
    
    //velocity button
    
    velBtn.setBounds(510, 68, 50, 20);
    
    //play / stop button
    playStopBtn.setBounds(460, 60, 35, 35);
    playStopBtnImageComp.setBounds(460, 60, 35, 35);
    
    float space = 130 / 5;
    myBtn_one.setBounds(2 + (space * 1) - 25, 300, 65, 95);
    myBtn_two.setBounds(2 + (space * 4) - 25, 300, 65, 95);
    myBtn_three.setBounds(469, 300, 65, 95);
    myBtn_four.setBounds(getWidth() - 53, 300, 65, 95);
    
    
    //Sphere instances
    for (auto& sphere : vectorOfSpheres)
    {
        sphere->setBounds (getLocalBounds());
    }
    
    //Sphere position sliders
    xPos_Slider.setBounds((space * 1) - 10, getHeight() / 2 - 100, 20, 200);
    yPos_Slider.setBounds((space * 4) - 10, getHeight() / 2 - 100, 20, 200);
    
    constXOffSet.setBounds(486, getHeight() / 2 - 100, 20, 200);
    constYOffSet.setBounds(getWidth() - 36, getHeight() / 2 - 100, 20, 200);
}

void Mandelbrot_pluginAudioProcessorEditor::btnOneIsClicked()
{
    switch(myBtn_one.getBtnCount()) {
        case 1:
            xMode = "sin";
            break;
        case 2:
            xMode = "cos";
            break;
        case 3:
            xMode = "noise";
            break;
        case 0:
            xMode = "slider";
            break;
    }
}

void Mandelbrot_pluginAudioProcessorEditor::btnTwoIsClicked()
{
    switch(myBtn_two.getBtnCount()) {
        case 1:
            yMode = "sin";
            break;
        case 2:
            yMode = "cos";
            break;
        case 3:
            yMode = "noise";
            break;
        case 0:
            yMode = "slider";
            break;
    }
}

void Mandelbrot_pluginAudioProcessorEditor::btnThreeIsClicked()
{
    switch(myBtn_three.getBtnCount()) {
        case 1:
            cxMode = "sin";
            break;
        case 2:
            cxMode = "cos";
            break;
        case 3:
            cxMode = "noise";
            break;
        case 0:
            cxMode = "slider";
            break;
    }
}

void Mandelbrot_pluginAudioProcessorEditor::btnFourIsClicked()
{
    switch(myBtn_four.getBtnCount()) {
        case 1:
            cyMode = "sin";
            break;
        case 2:
            cyMode = "cos";
            break;
        case 3:
            cyMode = "noise";
            break;
        case 0:
            cyMode = "slider";
            break;
    }
}
