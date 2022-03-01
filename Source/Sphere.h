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
class Sphere  : public juce::Component
{
public:
    Sphere(); 
    ~Sphere() override;
    
    void setPosition(float x_, float y_, float cx_, float cy_);
    void updatePosition(std::vector<std::unique_ptr<Sphere>>&);
    void limitSphere();
//    bool checkIntersection(double &rotatingArm, bool other);
    bool checkForPaint(double inc);
    double getXPos();
    double getYPos();
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    
    float x;
    float y;
    float cx;
    float cy;
    double spx;
    double spy;
    Coord cp;
    Coord mp;
    Coord vecA;
    Coord rp;
    Coord nVecB;
    Coord scaledPos;
    Coord nVecA;
    Coord newMP;
    Coord vecB;
    float magVecA;
    float phi;
    float dist0;
    float dist1;
    float dist2;
    double scalarProjection;
    int sphereRadius { 10 };
    int bordRad { 170 };
    float newX { 0 };
    float newY { 0 };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sphere)
};



