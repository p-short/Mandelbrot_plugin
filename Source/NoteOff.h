/*
  ==============================================================================

    NoteOff.h
    Created: 17 Mar 2022 9:24:48am
    Author:  Iansyst Loan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//NoteOff class counts how much time has pasted since a note on midi message, when a specified amount of time has past a note off midi message will be sent.

class NoteOff
{
public:
    
    NoteOff(int _midiChannel, int _midiNote, float _velo);
    ~NoteOff();
    void prepareInfo(double sampRate, int _no_noteDuration, float _noteOff_bpm); //pass tempo and note duration
    void countNoteOffDurration(int bufferSize);
    bool getIsClick();
    bool getLatch();
    void setLatch(bool changeBool);
    
    int noteOff_midiChannel;
    int noteOff_midiNote;
    float noteOff_velo;
    
private:
   
    int noteOff_totalSamps { 0 };
    double noteOff_sampRate;
    int noteOff_Interval { 0 };
    int noteOff_bpm { 120 };
    bool isClick = false;
    bool latch = true;
    float no_noteDuration { 3 };
};
