/*
  ==============================================================================

    NoteOff.cpp
    Created: 17 Mar 2022 9:24:48am
    Author:  Iansyst Loan

  ==============================================================================
*/

#include "NoteOff.h"

NoteOff::NoteOff(int _midiChannel, int _midiNote, float _velo)
{
    noteOff_midiChannel = _midiChannel;
    noteOff_midiNote = _midiNote;
    noteOff_velo = _velo;
}

NoteOff::~NoteOff()
{
}

void NoteOff::prepareInfo(double sampRate)
{
    noteOff_sampRate = sampRate;
    noteOff_Interval = 60.0 / noteOff_bpm * noteOff_sampRate;
}


void NoteOff::countNoteOffDurration(int bufferSize)
{
    noteOff_totalSamps += bufferSize;
    std::cout << noteOff_totalSamps << "\n";
    
    int noteOff_sampsRemaning = noteOff_totalSamps % noteOff_Interval;
    
    if ((noteOff_sampsRemaning + bufferSize) >= noteOff_Interval)
    {
        std::cout << "note duration has been met" << "\n";
        isClick = true;
    }
}

void NoteOff::getNoteOffMessage()
{
    //return add midi event note off when counter has met its condition
    noteOff_totalSamps = 0;
    std::cout << " NOTE OFF midi channel: " << noteOff_midiChannel << " midi note: " <<
    noteOff_midiNote << " velocity: " << noteOff_velo << "\n";
}

bool NoteOff::getIsClick()
{
    return isClick;
}

bool NoteOff::getLatch()
{
    return latch;
}

void NoteOff::setLatch(bool changeBool)
{
    latch = changeBool;
}
