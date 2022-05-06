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

//prepareInfo receives the samplerate from buffer
void NoteOff::prepareInfo(double sampRate, int _no_noteDuration)
{
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
    noteOff_Interval = (60.0 / noteOff_bpm * noteOff_sampRate) * no_noteDuration;
}

//when noteOff_Interval is surpassed isClick is set to true
void NoteOff::countNoteOffDurration(int bufferSize)
{
    noteOff_totalSamps += bufferSize;
//    std::cout << noteOff_totalSamps << "\n";
    
    int noteOff_sampsRemaning = noteOff_totalSamps % noteOff_Interval;
    
    if ((noteOff_sampsRemaning + bufferSize) >= noteOff_Interval)
    {
//        std::cout << "note duration has been met" << "\n";
        isClick = true;
    }
}

//getNoteOffMessage returns the midi note off event
void NoteOff::getNoteOffMessage()
{
    //reset noteOff_totalSamps to 0
    noteOff_totalSamps = 0;
    //std::cout << " NOTE OFF midi channel: " << noteOff_midiChannel << " midi note: " <<
    //noteOff_midiNote << " velocity: " << noteOff_velo << "\n";
}


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
