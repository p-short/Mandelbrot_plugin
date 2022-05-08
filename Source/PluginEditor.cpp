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

//the PluginEditor is where the GUI components are put together to produced the UI. Its also where the node circles are rendered on the the UI

//==============================================================================
Mandelbrot_pluginAudioProcessorEditor::Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor& p)
    : AudioProcessorEditor (&p), rotationValue(p.currentInfo), bpmFromAudioThread(p.currentBPM), audioProcessor (p)
{
    //the first bit of code ran in the PluginEditor checks to see whats the max value of node circle instances it has to produce. this way we allways start with the maximum amount of node cirlce instances and never have to worry about dynamically adding more during runtime.
    
    //the editorMaxVectorSize variable is assigned the size of the first vector in the "editorScalesVector" vector.
    auto editorMaxVectorSize = editorScalesVector[0].size();
    
    //using a for loop, loop through all the vectors inside "editorScalesVector". With each vector check if its size is bigger than the editorMaxVectorSize variable, if it is assign its value to the editorMaxVectorSize variable.
    for (int i = 0; i < editorScalesVector.size(); i++)
    {
        if (editorScalesVector[i].size() > editorMaxVectorSize)
        {
            editorMaxVectorSize = editorScalesVector[i].size();
        }
    }
    
    //now we know the maximum amount of node circles we need, create them and push them into a vector
    for (auto i = 0; i < editorMaxVectorSize; i++)
    {
        vectorOfSpheres.push_back(std::make_unique<Sphere>());
    }
    
    //this is to enable automation of certain parameters from within the plug-in host (your DAW)
    comboAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.tree, KEY_ID, noteSelection);
    
    //here is where I set up all the attributes of the indervidual GUI components.
    
    //-----SLIDERS-----
    
    //XPOS SLIDER
    //this slider moves the position of the node circles on the x axis
    //set all the sliders to the same colours
    
    getLookAndFeel().setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
    getLookAndFeel().setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    getLookAndFeel().setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    
    //make Slider vertical and linear
    xPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    //sliders range is from -1 to 1 and is stepped through in intervals of 0.001 so the outputs nice and smooth
    xPos_Slider.setRange(-1.0f, 1.0f, 0.001f);
    //set slider value to 0 in the middle of its range
    xPos_Slider.setValue(0.0f);
    //add listener to this slider instance, this is so the sliders values and be passed on when slider is moved.
    xPos_Slider.addListener(this);
    //make slider visible on the UI
    addAndMakeVisible(xPos_Slider);
    
    //YPOS SLIDER
    //this slider moves the position of the node circles on the x axis
    yPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    yPos_Slider.setRange(-1.0f, 1.0f, 0.001f);
    yPos_Slider.setValue(0.0f);
    yPos_Slider.addListener(this);
    addAndMakeVisible(yPos_Slider);
    
    //CONST X OFFSET SLIDER
    //this slider moves the offset of the node circles on the x axis
    constXOffSet.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    constXOffSet.setRange(-1.0f, 1.0f, 0.001f);
    constXOffSet.setValue(0.0f);
    constXOffSet.addListener(this);
    addAndMakeVisible(constXOffSet);
    
    //CONST Y OFFSET SLIDER
    //this slider moves the offset of the node circles on the y axis
    constYOffSet.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    constYOffSet.setRange(-1.0f, 1.0f, 0.001f);
    constYOffSet.setValue(0.0f);
    constYOffSet.addListener(this);
    addAndMakeVisible(constYOffSet);
    
    //BMP SLIDER
    //this slider increases / decreases the speed the arm rotates at
    BPM_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    BPM_Slider.setTextValueSuffix(" BPM");
    BPM_Slider.setRange(0, 300, 1);
    BPM_Slider.setValue(120);
    BPM_Slider.setTextBoxIsEditable(false);
    BPM_Slider.addListener(this);
    addAndMakeVisible(BPM_Slider);
    
    
    //NOTE AMOUNT SLIDER
    // this slider increases / decreases the amount the node circles rendered to the UI
    noteAmount.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    noteAmount.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    noteAmount.setRange(0, 7, 1);
    noteAmount.setValue(7);
    noteAmount.addListener(this);
    addAndMakeVisible(noteAmount);
    
    //NOTE DURATION SLIDER
    //this slider selects how long you want inbertween midi note on / note off messages
    noteDuration_slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    noteDuration_slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    noteDuration_slider.setRange(0, 6, 1);
    noteDuration_slider.setValue(3);
    noteDuration_slider.addListener(this);
    addAndMakeVisible(noteDuration_slider);
    
    
    //-----COMBOBOXES-----
    
    //the comboboxes are present at the top left hand corner of the UI, if you click them they present a drop down menu containing multiple options, to select one of the options click the one you want to select.
    
    
    //NOTE SELECTION COMBOBOX
    //this comobox lets the user select the root note of the scale
    //addItem puts items in to the combobox, it takes a string to display on the UI and an index id
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
    //place text in centre of combobox
    noteSelection.setJustificationType(juce::Justification::centred);
    //set colour
    noteSelection.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
    //add listener to this this combobox
    noteSelection.addListener(this);
    //set a default value to display when the plugin is opened
    noteSelection.setSelectedId(1);
    //make combobox visible on the UI
    addAndMakeVisible(noteSelection);
    
    //OCTAVE SELECTION COMBOBOX
    //this combobox lets the user select what octave they want the notes to be in
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
    
    //SCALE SELECTION COMBOBOX
    // this combobox lets the user select what musical scale they want to output
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
    
    //MIDI CHANNEL SELECTION
    //this combobox lets the user select what midi channel they want their midi data sent to
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
    
    //-----BUTTONS-----
    
    //SYNCH BUTTON
    //synch button synchs the plugins tempo to that of the DAWs
    //button state is set to flase, you need to click on the sych button to enable it
    synchBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
    //set buttons colour
    synchBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
    //add listerner
    synchBtn.addListener(this);
    //make button visible on the UI
    addAndMakeVisible(synchBtn);
    
    
    //DOUBLE SPEED BUTTON
    //double the speed of the rotating arm
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
    
    myBtn_one.setModName("X", 8);
    myBtn_two.setModName("Y", 8);
    myBtn_three.setModName("XOS", 0);
    myBtn_four.setModName("YOS", 0);
    
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
    g.drawLine(0, 0, borderRadius * cos(rotation), borderRadius * sin(rotation), 1);
    
    
    //scale returns 0, 1, 2, 3
    
//    std::cout << "x : " << vectorOfSpheres[14]->getXPos() << "\n" <<
//    "y : " <<vectorOfSpheres[14]->getYPos() << "\n";
    
    if (scale <= editorScalesVector.size() - 1)
    {
    
        for (auto i = 0; i < noteAmount.getValue(); i++) //editorScalesVector[scale].size()
        {
            if (vectorOfSpheres[i]->checkForPaint(rotation) && isPlaying)
            {
                g.setColour(juce::Colour::fromFloatRGBA (0.996f, 0.509f, 0.549f, 1.0f));
            }
            
            else
            {
                g.setColour(juce::Colour::fromFloatRGBA (0.996f, 0.509f, 0.549f, 0.5f));
            }
            
            g.fillEllipse((vectorOfSpheres[i]->getXPos() * smallerBorderRadius) - sphereRad,
                          (vectorOfSpheres[i]->getYPos() * smallerBorderRadius) - sphereRad,
                          8 * 2, 8 * 2);
            
            //line of scalar projection
//            g.setColour(juce::Colours::green);
//            g.drawLine(vectorOfSpheres[i]->getXPos() * 160, vectorOfSpheres[i]->getYPos() * 160,
//                       vectorOfSpheres[i]->spx, vectorOfSpheres[i]->spy, 3);
        }
        
        for (auto j = 0; j < noteAmount.getValue() -1; j++)
        {
            g.setColour(juce::Colours::black);
            g.drawLine(vectorOfSpheres[j]->getXPos() * smallerBorderRadius,
                       vectorOfSpheres[j]->getYPos() * smallerBorderRadius,
                       vectorOfSpheres[j +1]->getXPos() * smallerBorderRadius,
                       vectorOfSpheres[j +1]->getYPos() * smallerBorderRadius, 0.5);
        }
    }
    

    
    g.restoreState();
    
    //scale selection label
    g.setColour(juce::Colours::white);
    g.drawText("Scale Selection", 75, 4, 200, 20, juce::Justification::topLeft);
    
    g.setColour(juce::Colours::black);
    g.drawText("Root", 22, 20, 200, 20, juce::Justification::topLeft);
    g.drawText("Oct", 84, 20, 200, 20, juce::Justification::topLeft);
    g.drawText("Scale", 158, 20, 200, 20, juce::Justification::topLeft);
    
    //midi channel selection label
    g.drawText("Midi Channel", 26, 58, 200, 20, juce::Justification::topLeft);
    g.drawText("Note Amount", 134, 58, 200, 20, juce::Justification::topLeft);
    
    //Rotation speed label
    g.setColour(juce::Colours::white);
    g.drawText("Rotation Speed", 375, 4, 200, 20, juce::Justification::centred);
    
    g.setColour(juce::Colours::black);
    
    //note duration slider label
    
    g.drawText("Note Duration", 410, 68, 200, 20, juce::Justification::topLeft);
    
    //x label
    g.saveState();
    g.addTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 4, 216));
    g.drawText("x axis", 4, 216, 100, 50, juce::Justification::topLeft);
    g.restoreState();
    
    //y label
    g.saveState();
    g.addTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 109, 216));
    g.drawText("y axis", 109, 216, 100, 50, juce::Justification::topLeft);
    g.restoreState();
    
    //x offset label
    g.saveState();
    g.addTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 473, 222));
    g.drawText("x offset", 473, 222, 100, 50, juce::Justification::topLeft);
    g.restoreState();
    
    //y offset label
    g.saveState();
    g.addTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 579, 222));
    g.drawText("y offset", 579, 222, 100, 50, juce::Justification::topLeft);
    g.restoreState();
    
    //center of the note duration component
    float centreX = noteDuration_slider.getX() + 30;
    float centreY = noteDuration_slider.getY() + 30;
    float myAng = M_PI / 2 + M_PI / 4;
    float num2DivBy  = 6;
    float rangeInRad = 4.71238898;
    float step = rangeInRad / num2DivBy;
    float textRad = 32.5;
    
    //array of strings that indercate the note duration these are then drawn around the dial corresponding with the dials steps.
    std::array<std::string, 7> stringArray = {"Imp", "1/32", "1/16", "1/8", "1/4", "1/2", "1"};
    
    g.saveState();
    g.setFont(10.0);
    for (int i = 0; i < 7; i++)
    {
        g.drawText(stringArray[i], centreX + textRad * cos(myAng + i * step) - 10, centreY + textRad * sin(myAng + i * step) - 10, 20, 20, juce::Justification::horizontallyCentred);
    }
}


void Mandelbrot_pluginAudioProcessorEditor::timerCallback()
{
    repaint();
    
    rotation = rotationValue.getValue();
    
    //is synch button is pressed disable bpm slider from being dragged by mouse and set the sliders values to the DAWs tempo from audio thread.
    if (synchBool)
    {
        BPM_Slider.setEnabled(false);
        BPM_Slider.setValue(bpmFromAudioThread.getBPM());
        BPM_Slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126).withAlpha(0.5f));
        BPM_Slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68).withAlpha(0.5f));
        BPM_Slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68).withAlpha(0.5f));
    }
    else
    {
        BPM_Slider.setEnabled(true);
        BPM_Slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
        BPM_Slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
        BPM_Slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
    }
    
    myBtn_one.disableSliders(xMode);
    myBtn_two.disableSliders(yMode);
    myBtn_three.disableSliders(cxMode);
    myBtn_four.disableSliders(cyMode);
    
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
        xPos_Slider.setEnabled(false);
        xPos_Slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126).withAlpha(0.5f));
        xPos_Slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68).withAlpha(0.5f));
        xPos_Slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68).withAlpha(0.5f));
        
        
    }
    
    else
    {
        xFreq = 0;
        xPos_Slider.setEnabled(true);
        xPos_Slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(225, 40, 126));
        xPos_Slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(36, 44, 68));
        xPos_Slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(36, 44, 68));
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
    
    audioProcessor.apNoteAmount = noteAmount.getValue();
    audioProcessor.apNoteDuration = noteDuration_slider.getValue();
    audioProcessor.apBPM = BPM_Slider.getValue();
    
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
        
        //each time a different scale is selected the note amount slider range / value is set to the length of the current scale
        unsigned long noteAmountMaxVal = editorScalesVector[scale].size();
        noteAmount.setRange(0, noteAmountMaxVal, 1);
        noteAmount.setValue(noteAmountMaxVal);
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
            synchBool = true;
            audioProcessor.apSynch = true;
        }
        
        else if (synchBtnOnState == OnState::on)
        {
            synchBtnOnState = OnState::off;
            synchBtn.setToggleState(false, juce::NotificationType::dontSendNotification);
            synchBtn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
            synchBool = false;
            audioProcessor.apSynch = false;
        }
    }
    
    if (button == &doubleSpeedBtn)
    {
        divBy = 2;
        audioProcessor.apPlaybackSpeed = 128;
        
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
        audioProcessor.apPlaybackSpeed = 64;
        
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
        audioProcessor.apPlaybackSpeed = 32;
        
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
    int xSpacing = 5;
    int ySpacing = 25;
    noteSelection.setBounds(xSpacing, 10 + ySpacing, 60, 20);
    octaveSelection.setBounds(60 + xSpacing, 10 + ySpacing, 60, 20);
    scaleSelection.setBounds(120 + xSpacing, 10 + ySpacing, 100, 20);
    midiChan.setBounds(xSpacing, 40 + ySpacing + 8, 120, 20);
    noteAmount.setBounds(120 + xSpacing, 40 + ySpacing + 8, 100, 20); //360, 10, 240, 20
   
    int anotherYSpacing = 15;
    int anotherXSpacing = 20;
    //speed slider
    BPM_Slider.setBounds(360, 10 + anotherYSpacing, 240, 20);
    
    //synch button
    synchBtn.setBounds(360 + anotherXSpacing, 30 + anotherYSpacing, 50, 20);
    
    //double speed button
    doubleSpeedBtn.setBounds(410 + anotherXSpacing, 30 + anotherYSpacing, 50, 20);
    
    //normal speed button
    normalSpeedBtn.setBounds(460 + anotherXSpacing, 30 + anotherYSpacing, 50, 20);
    
    //half speed button
    halfSpeedBtn.setBounds(510 + anotherXSpacing, 30 + anotherYSpacing, 50, 20);
    
    //note duration slider
    noteDuration_slider.setBounds(425, 90, 60, 60);
    
    //velocity button
    
    velBtn.setBounds(275, 370, 50, 20);
    
    //play / stop button
    playStopBtn.setBounds(277.5, 20, 45, 45);
    playStopBtnImageComp.setBounds(277.5, 20, 45, 45);
    
    float space = 130 / 5; // 26
    myBtn_one.setBounds(3, 280, 65, 120);
    myBtn_two.setBounds(81, 280, 65, 120);
    myBtn_three.setBounds(454, 280, 65, 120);
    myBtn_four.setBounds(532, 280, 65, 120);
    
    
    //Sphere instances
    for (auto& sphere : vectorOfSpheres)
    {
        sphere->setBounds (getLocalBounds());
    }
    
    //Sphere position sliders
    xPos_Slider.setBounds((space * 1) - 10, getHeight() / 2 - 80, 20, 165);
    yPos_Slider.setBounds((space * 4) - 10, getHeight() / 2 - 80, 20, 165);
    
    constXOffSet.setBounds(486, getHeight() / 2 - 80, 20, 165);
    constYOffSet.setBounds(getWidth() - 36, getHeight() / 2 - 80, 20, 165);
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
