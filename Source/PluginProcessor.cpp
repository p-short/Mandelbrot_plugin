/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Mandelbrot_pluginAudioProcessor::Mandelbrot_pluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    //store the size of first vector in 2d vector in variable
    auto maxVectorSize = scalesVector[0].size();
    
    //loop through the size of each vector within the 2d vector and
    //if the current vector size is greater than the value stored in the variable
    //swap.
    for (int i = 0; i < scalesVector.size(); i++)
    {
        if (scalesVector[i].size() > maxVectorSize)
        {
            maxVectorSize = scalesVector[i].size();
            //max VectorSize currently would be 15 (0 - 14);
        }
    }
    
    //use the maximum vector size stored in variable to produce
    //an aproprate amount of sphere instances
    for (int i = 0; i < maxVectorSize; i++)
    {
        sphereLogicVector.push_back(std::make_unique<SphereLogic>());
    }
    
//        std::cout << scalesVector[4].size() << "\n";
}

Mandelbrot_pluginAudioProcessor::~Mandelbrot_pluginAudioProcessor()
{
}

//==============================================================================
const juce::String Mandelbrot_pluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Mandelbrot_pluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Mandelbrot_pluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Mandelbrot_pluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Mandelbrot_pluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Mandelbrot_pluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Mandelbrot_pluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Mandelbrot_pluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Mandelbrot_pluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void Mandelbrot_pluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Mandelbrot_pluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
//    noteOff.prepareInfo(sampleRate);
}

void Mandelbrot_pluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Mandelbrot_pluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Mandelbrot_pluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
//    processedBuffer.clear();
    
    auto bufferForClass = buffer.getNumSamples();
    
//    noteOff.setNoteOnInfo(1, 65, 0.5);
    
//    if (!noteOff.getIsClick())
//    {
//        noteOff.countNoteOffDurration(bufferForClass);
//    }
    
    
    if (apScale <= scalesVector.size() - 1)
    {
//        std::cout << apScale << "\n";
    
    for (int i = 0; i < scalesVector[apScale].size(); i++)
    {
        sphereLogicVector[i]->setPosition(apx_pos, apy_pos, apcx_pos, apcy_pos);
        sphereLogicVector[i]->updatePosition(sphereLogicVector);
        sphereLogicVector[i]->limitSphere();

            if (sphereLogicVector[i]->checkIntersection(inc, sphereLogicVector[i]->getSphereBool()) && apIsPlaying)
            {
                //add midi event
                midiNote = apRootNote + scalesVector[apScale][i];
                float velocity = (apIsVel) ? sphereLogicVector[i]->getMag() : 1.0f;
                auto onMessage = juce::MidiMessage::noteOn(apMidiChan, midiNote, velocity);
//                auto offMessage = juce::MidiMessage::noteOff(apMidiChan, midiNote, velocity);
                auto onTimeStamp = onMessage.getTimeStamp();
                
                midiMessages.addEvent(onMessage, onTimeStamp);
                
                //push NoteOff instance into vector;
                noteOffVector.push_back(*new NoteOff(apMidiChan, midiNote, velocity));
                
//                auto offTimeStamp = offMessage.getTimeStamp();
//                midiMessages.addEvent(offMessage, offTimeStamp);
                
                sphereLogicVector[i]->setSphereBool(false);
            }
        }
        
        for (int i = 0; i < noteOffVector.size(); i++)
        {
            if (!noteOffVector[i].getIsClick())
            {
                noteOffVector[i].prepareInfo(getSampleRate());
                noteOffVector[i].countNoteOffDurration(bufferForClass);
            }
            
            else if (noteOffVector[i].getIsClick() && noteOffVector[i].getLatch())
            {
                //have function return a midi event insted of printing to the consle.
                noteOffVector[i].getNoteOffMessage();
                noteOffVector[i].setLatch(false);
            }
        }
        
       
        //check of note is off
//        if (noteOff.getIsClick() && noteOff.getLatch())
//        {
//            noteOff.getNoteOffMessage();
//            noteOff.setLatch(false);
//        }
    }
    
    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage currentMessage;
    int samplePos;
    
    while (it.getNextEvent(currentMessage, samplePos))
    {
        
        std::cout << currentMessage.getDescription() << "\n";
        
//        if (currentMessage.isNoteOnOrOff ())
//        {
//            int note = currentMessage.getNoteNumber ();
//            currentMessage.setNoteNumber (note); // transpose it
//        }
//        if (currentMessage.isNoteOnOrOff())
//        {
//            currentMessage.setNoteNumber(50);
//        }
//        processedBuffer.addEvent(currentMessage, samplePos);
    }
//    midiMessages.swapWith(processedBuffer);
}

//==============================================================================
bool Mandelbrot_pluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Mandelbrot_pluginAudioProcessor::createEditor()
{
    return new Mandelbrot_pluginAudioProcessorEditor (*this);
}

//==============================================================================
void Mandelbrot_pluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Mandelbrot_pluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//void Mandelbrot_pluginAudioProcessor::paint (juce::Graphics& g)
//{
//    g.setColour(juce::Colours::black);
//    g.drawEllipse(200, 200, 40, 40, 1);
//}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Mandelbrot_pluginAudioProcessor();
}
