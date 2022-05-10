/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

//alot of the code is boiler plate that JUCE produces when you choose to write a plugin. The only part of the file I've added upon is the process block.

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
                       ),
//use in the automation of combobox item from DAW
tree(*this, nullptr, "PARAMETER", { std::make_unique<juce::AudioParameterChoice> (KEY_ID, KEY_NAME, keyIdArray, 0, KEY_NAME, nullptr, nullptr) })

#endif
{
    //code to determine maximum number of note instances to produce

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
        midiNodeCircleVector.push_back(std::make_unique<MidiNodeCircle>());
    }
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

//process block this is where all the midi output is handled.
void Mandelbrot_pluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //firstly clear buffer
    buffer.clear();
    //only perform this block of code if playhead is detected. This is where alot of the information from the DAW is accessible to the plugi.n
    if (auto* myPlayHead = getPlayHead())
    {
        //store DAW info in myPlayHead pointer
        juce::AudioPlayHead::CurrentPositionInfo info;
        myPlayHead->getCurrentPosition(info);
        

        
       
        if (!apSynch)
        {
            //if synch button is false then the bpm is set by the bpm GUI slider
            tempBPM = apBPM;
            //if play button is on / off set apIsPlaying to true / false
            if (apisPlayBtn)
            {
                apIsPlaying = true;
            }
            else
            {
                apIsPlaying = false;
            }
        }
        
        else if (apSynch)
        {
            //if synch button is true then bpm is set to the value of the DAWs bpm
            tempBPM = info.bpm;
            //this is then passed to the gui thread so the slider value is chaged while its disabled
            currentBPM.store(info.bpm);
            //set apIsPlaying according to DAWs play / stop button
            if (apSynch && !info.isPlaying)
            {
                apIsPlaying = false;
            }
            else if (apSynch && info.isPlaying)
            {
                apIsPlaying = true;
            }
            //this is then passed to the gui thread
            currentBtnState.store(apIsPlaying);
        }
        

        
    
        //get info from host, calculate what the rotation speed should be then send it to gui thread form the animations
        double numSampsInBar = (60 / tempBPM * getSampleRate()) / apPlaybackSpeed; // 128 = double time. 64 = normal 32 = halfspeed.
        double increment =  tp / numSampsInBar;
        currentInfo.store(startAng);
        
        //if play is pressed on the DAW or on plugin, start rotation.
        startAng = (apIsPlaying) ? startAng += increment : startAng = -M_PI / 2;
    }
    
    
    auto bufferForClass = buffer.getNumSamples();
    

    //loop up untill apNoteAmount this can be changed in runtime via noteAmountSlider
    for (int i = 0; i < apNoteAmount; i++) //scalesVector[apScale].size()
    {
        //loop through and set, update and limit the node circles. Note that unlike the GUI thread this isn't actually rendering anything. its purley for collision detection so we know when to trigger notes or note.
        midiNodeCircleVector[i]->setPosition(apx_pos, apy_pos, apcx_pos, apcy_pos);
        midiNodeCircleVector[i]->updatePosition(midiNodeCircleVector);
        midiNodeCircleVector[i]->limitNodeCircle();

        //check if rotating arm has collided with any node circle
            if (midiNodeCircleVector[i]->checkIntersection(startAng, midiNodeCircleVector[i]->getSphereBool()) && apIsPlaying)
            {
                //when hit, use the index of i to select the relevant integer (representing intervals in a scale) from the relevant vector (representing musical scales) + the rootnote to produce the midinote to be outputted
                midiNote = apRootNote + scalesVector[apScale][i];
                //when button is pressed "velocity" is set to the magnitude of the position of i from the centre of the border circle to its edges, this returns a normalised float, 0 if its in the centre and 1 if its flush with the border circles edge. If button not pressed a fixed velocity of 1 is set.
                float velocity = (apIsVel) ? midiNodeCircleVector[i]->getMag() : 1.0f;
                
                //onMessage is a juce midinote object waiting to be added to the midibuffer
                auto onMessage = juce::MidiMessage::noteOn(apMidiChan, midiNote, velocity);
                //onTimeStamp returns the moment in time the collision was detected so it can be placed at the appropriate place within the midi buffer
                auto onTimeStamp = onMessage.getTimeStamp();
                //add the midi note on message to the midi buffer, thus outputted the relevant midi note.
                midiMessages.addEvent(onMessage, onTimeStamp);
                
                //store midinote off message
                auto instantOffMessage = juce::MidiMessage::noteOff(apMidiChan, midiNote, velocity);
                auto instantOffTimeStamp = instantOffMessage.getTimeStamp();
                
                //if the note duration slider on the GUI is set to 0 add the midi note off message to the midi buffer directly after the midi note on message. if the note duration slider is at one then its set to "impulse mode" this it to trigger a instantaneous gate (this is useful for triggering synthesisers / software synths that have their own ADSR).
                if (apNoteDuration == 0)
                {
                    midiMessages.addEvent(instantOffMessage, instantOffTimeStamp);
                }
                else
                {
                    //if apNoteDuration is anything other than 0 push instance of noteOff class in to vector. This will return a midi note off message after a specified amount of time.
                    noteOffVector.push_back(std::make_unique<NoteOff>(apMidiChan, midiNote, velocity));
                }
                //after adding the midi note to the midi buffer set this node circle instance but to false so its only triggered once per collision.
                midiNodeCircleVector[i]->setSphereBool(false);
            }
        }
        
        //loop through noteOffvector and constantly check if duration of time has passed to set back note off midi message to midi buffer
        for (int i = 0; i < noteOffVector.size(); i++)
        {
            //
            if (!noteOffVector[i]->getIsClick())
            {
                //caluculate when midi note off should be returned
                noteOffVector[i]->prepareInfo(getSampleRate(), apNoteDuration, tempBPM);
                //start counter
                noteOffVector[i]->countNoteOffDurration(bufferForClass);
            }

            else if (noteOffVector[i]->getIsClick() && noteOffVector[i]->getLatch())
            {
                //when duration of time is met grab the right instance index and return a midi note off message
                auto offMessage = juce::MidiMessage::noteOff(noteOffVector[i]->noteOff_midiChannel,
                                                             noteOffVector[i]->noteOff_midiNote,
                                                             noteOffVector[i]->noteOff_velo);
                auto offTimeStamp = offMessage.getTimeStamp();
                //add message to midi buffer
                midiMessages.addEvent(offMessage, offTimeStamp);
                //set instance to false so its only triggered once
                noteOffVector[i]->setLatch(false);
                //if vector is greater than one remove first item so vector dosent take up too much memory during runtime
                if (noteOffVector.size() > 1)
                {
                    noteOffVector.erase(noteOffVector.begin());
                }
            }
        }

    
    //iterator constantly loops through the midi buffer and keeps track of where it is within the buffer at any one time
    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage currentMessage;
    int samplePos;
    //while loop checks for midi note events and places them inside the buffer acording to thier timestamp
    while (it.getNextEvent(currentMessage, samplePos))
    {
        

    }
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
