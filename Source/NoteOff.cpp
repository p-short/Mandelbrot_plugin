/*
  ==============================================================================

    NoteOff.cpp
    Created: 17 Mar 2022 9:24:48am
    Author:  Iansyst Loan

  ==============================================================================
*/

#include "NoteOff.h"

//constructor takes and stores the midi channel, midi note and midi velocity
NoteOff::NoteOff(int _midiChannel, int _midiNote, float _velo)
{
    noteOff_midiChannel = _midiChannel;
    noteOff_midiNote = _midiNote;
    noteOff_velo = _velo;
}

NoteOff::~NoteOff()
{
}

//prepareInfo receives the samplerate from buffer. It also thats the int value from the note duration slider from the GUI thread and selects the appropriate value to set no_noteDuration.
void NoteOff::prepareInfo(double sampRate, int _no_noteDuration, float _noteOff_bpm)
{
    //picks appropriate value according to int slider from GUI thread
    switch(_no_noteDuration)
    {
        case 1:
            no_noteDuration = 0.25;
            break;
        case 2:
            no_noteDuration = 0.5;
            break;
        case 3:
            no_noteDuration = 1;
            break;
        case 4:
            no_noteDuration = 2;
            break;
        case 5:
            no_noteDuration = 4;
            break;
        case 6:
            no_noteDuration = 8;
            break;
    }
    noteOff_sampRate = sampRate;
    noteOff_bpm = _noteOff_bpm;
    // calculate note interval
    noteOff_Interval = (60.0 / noteOff_bpm * noteOff_sampRate) * no_noteDuration;
}

//when noteOff_Interval is surpassed isClick is set to true
void NoteOff::countNoteOffDurration(int bufferSize)
{
    //count up from each sample in bufferSize
    noteOff_totalSamps += bufferSize;
    int noteOff_sampsRemaning = noteOff_totalSamps % noteOff_Interval;
    //when samps remaining is bigger note interval set click to true and rest total samps count to 0
    if ((noteOff_sampsRemaning + bufferSize) >= noteOff_Interval)
    {
        isClick = true;
        noteOff_totalSamps = 0;
    }
}

//getNoteOffMessage returns the midi note off event

//returns isClick bool
bool NoteOff::getIsClick()
{
    return isClick;
}
//returns latch bool
bool NoteOff::getLatch()
{
    return latch;
}
//sets latch bool
void NoteOff::setLatch(bool changeBool)
{
    latch = changeBool;
}
