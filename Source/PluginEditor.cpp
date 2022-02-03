/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Spheres.h"

//==============================================================================
Mandelbrot_pluginAudioProcessorEditor::Mandelbrot_pluginAudioProcessorEditor (Mandelbrot_pluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (600, 400);
    addAndMakeVisible(sphere);
    
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
    
    //scale selection combobox
    
    scaleSelection.addItem("Major 1 Oct", 1);
    scaleSelection.addItem("Major 2 Oct", 2);
    scaleSelection.addItem("Minor 1 Oct", 3);
    scaleSelection.addItem("Minor 2 Oct", 4);
    scaleSelection.setJustificationType(juce::Justification::centred);
    
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
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setOrigin(getWidth() / 2, getHeight() / 2);
    g.drawEllipse(0 - borderRadius, 0 - borderRadius, borderRadius * 2, borderRadius * 2, 2);
    g.drawLine(0, 0, borderRadius * cos(t), borderRadius * sin(t), 2);
}

// calling repaint 60 time a second on *this* and  Sphere instance
void Mandelbrot_pluginAudioProcessorEditor::timerCallback()
{
    /*
     its probaly here I would put the algor that produces the spheres and there positions
     acording to the mandelbrot maths.
     
     first i would create an Sphere instance with the xSlider and ySlider values as the x & y
     params and push them into an array.
     
     then in a for loop ill use that x and y position of that first instance to inform the next
     X amount of iterations.
     
     i.e.---------------------
     
     int x = 5;
     
     Spheres firstInstance(xSliderVals, ySliderVals);
     Spheres const_val(0.0, 0.0);
    
     myArray.push_back(firstInstance);
     
     for (int i = 0; i < x; i++) {
         myArray.push_back(new Sphere(pow(myArray[i].x, 2) +
                           -pow(myArray[i].y, 2) +
                           (pow(const_val.x, 2) + -pow(const_val.y, 2)),
                           2 * myArray[i].x * myArray[i].y +
                           2 * const_val.x * const_val.y));
     }
     
     this will be expensive cus its filling a vector with 4 instances then emptying the vector every drawloop
     this is how i have in working in my JS version.
    */
    repaint();
    sphere.repaint();
    t += 0.01;
    //insted of listening for slider values change im just passing thier values directly to the set position function
    sphere.setPosition(xPos_Slider.getValue(), yPos_Slider.getValue());
}

// listen for slider value changes, pass them to variables in Sphere instance
void Mandelbrot_pluginAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == & xPos_Slider)
    {
        //sphere.xSliderVals = xPos_Slider.getValue();
    }
    else if (slider == & yPos_Slider)
    {
        //sphere.ySliderVals = yPos_Slider.getValue();
    }
}

void Mandelbrot_pluginAudioProcessorEditor::resized()
{
    // set positions
    sphere.setBounds(getLocalBounds());
    xPos_Slider.setBounds(getWidth() / 12, getHeight() / 2 - 100, 20, 200);
    yPos_Slider.setBounds(getWidth() / 12 + 50, getHeight() / 2 - 100, 20, 200);
}
