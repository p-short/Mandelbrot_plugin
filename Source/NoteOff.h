/*
  ==============================================================================

    NoteOff.h
    Created: 17 Mar 2022 9:24:48am
    Author:  Iansyst Loan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class NoteOff
{
public:
    
    NoteOff(int _midiChannel, int _midiNote, float _velo);
    ~NoteOff();
    void prepareInfo(double sampRate);
    void countNoteOffDurration(int bufferSize);
    void getNoteOffMessage();
    bool getIsClick();
    bool getLatch();
    void setLatch(bool changeBool);
    
private:
    int noteOff_midiChannel;
    int noteOff_midiNote;
    float noteOff_velo;
    int noteOff_totalSamps { 0 };
    double noteOff_sampRate;
    int noteOff_Interval { 0 };
    int noteOff_bpm { 120 };
    bool isClick = false;
    bool latch = true;
};
