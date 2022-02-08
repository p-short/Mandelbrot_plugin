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
    setSize (600, 400);
    
    for (auto i = 0; i < 7; i++)
    {
        vectorOfSpheres.push_back(std::make_unique<Sphere>());
        addAndMakeVisible(*vectorOfSpheres.back());
    }
    
    resized();
    
//if you comment out "addAndMakeVisible(sphere)", "sphere.setPosition(xPos_Slider.getValue(), yPos_Slider.getValue())" in timerCallback() and "sphere.setBounds(getLocalBounds())" in resize. Its drawn and animated to the UI, how come my vector of spheres is not drawing to the UI at all, I would expect to see 4 spheres drawn over each over in the center of the UI.

    //addAndMakeVisible(sphere);
   
    //add sliders, set range, value ect..
    xPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    xPos_Slider.setRange(-1.0f, 1.0f, 0.01f);
    xPos_Slider.setValue(0.0f);
    xPos_Slider.addListener(this);
    addAndMakeVisible(xPos_Slider);
    
    //ypos slider
    yPos_Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    yPos_Slider.setRange(-1.0f, 1.0f, 0.001f);
    yPos_Slider.setValue(0.0f);
    yPos_Slider.addListener(this);
    addAndMakeVisible(yPos_Slider);
    
    //need to finish comboboxs
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
//    noteSelection.addListener(this);
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
//    octaveSelection.addListener(this);
    addAndMakeVisible(octaveSelection);
    
    //scale selection combobox
    
    scaleSelection.addItem("Major 1 Oct", 1);
    scaleSelection.addItem("Major 2 Oct", 2);
    scaleSelection.addItem("Minor 1 Oct", 3);
    scaleSelection.addItem("Minor 2 Oct", 4);
    scaleSelection.setJustificationType(juce::Justification::centred);
//    scaleSelection.addListener(this);
    addAndMakeVisible(scaleSelection);
    
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
    g.fillAll (juce::Colours::dimgrey);
    g.setColour(juce::Colours::black);
    g.drawEllipse(300 - borderRadius, 200 - borderRadius, borderRadius * 2, borderRadius * 2, 2);
    g.setOrigin(getWidth() / 2, getHeight() / 2);
    g.drawLine(0, 0, borderRadius * cos(t), borderRadius * sin(t), 2);
   
}


void Mandelbrot_pluginAudioProcessorEditor::timerCallback()
{
    repaint();
   
    vectorOfSpheres[0]->setPosition(xPos_Slider.getValue(), yPos_Slider.getValue());
    
//    for (int i = 0; i < vectorOfSpheres.size() - 1; i++) {
//        vectorOfSpheres[i + 1]->x = (pow(vectorOfSpheres[i]->x, 2) + -pow(vectorOfSpheres[i]->y, 2)) +
//        (pow(cVec.x, 2) + -pow(cVec.y, 2));
//        vectorOfSpheres[i + 1]->y = 2 * vectorOfSpheres[i]->x * vectorOfSpheres[i]->y + 2 * cVec.x * cVec.y;
//    }
    for (int i = 0; i < vectorOfSpheres.size(); i++)
    {
        vectorOfSpheres[i]->updatePosition(vectorOfSpheres);
    }
    
    t += 0.05;
    
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
}

void Mandelbrot_pluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    
}

void Mandelbrot_pluginAudioProcessorEditor::resized()
{
    // set positions
    noteSelection.setBounds(10, 10, 50, 20);
    for (auto& sphere : vectorOfSpheres)
    {
        sphere->setBounds (getLocalBounds());
    }
    //sphere.setBounds(getLocalBounds());
//    my_test.setBounds(0, 0, getWidth(), getHeight() / 2);
    xPos_Slider.setBounds(getWidth() / 12, getHeight() / 2 - 100, 20, 200);
    yPos_Slider.setBounds(getWidth() / 12 + 50, getHeight() / 2 - 100, 20, 200);
}
