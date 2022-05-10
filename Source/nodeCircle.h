/*
  ==============================================================================

    Spheres.h
    Created: 29 Jan 2022 3:24:01pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "myFunctions.h"

//==============================================================================
/*
*/

//inherit form component class
class NodeCircle : public juce::Component
{
public:
    NodeCircle();
    ~NodeCircle() override;
    
    //Node circle methods
    void setPosition(float x_, float y_, float cx_, float cy_);
    void updatePosition(std::vector<std::unique_ptr<NodeCircle>>&);
    void limitPosition();
    bool checkForPaint(double inc);
    double getXPos();
    double getYPos();
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    
    //instances of Coord objects (see myFunctions .h & .cpp)
    Coord cp;
    Coord mp;
    Coord vecA;
    Coord rp;
    Coord nVecB;
    Coord scaledPos;
    Coord nVecA;
    Coord newMP;
    Coord vecB;
    
    //variable used in the calculation of nodeCircles position
    float x;
    float y;
    float cx;
    float cy;
    double spx;
    double spy;
    float magVecA;
    float phi;
    float dist0;
    float dist1;
    float dist2;
    double scalarProjection;
    int nodeCircleRadius { 7 };
    int bordRad { 140 };
    float newX { 0 };
    float newY { 0 };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NodeCircle)
};



