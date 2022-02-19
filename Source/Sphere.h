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
    Sphere(); //float _x, float _y
    ~Sphere() override;
    
    void setPosition(float x_, float y_, float cx_, float cy_);
    void updatePosition(std::vector<std::unique_ptr<Sphere>>&);
    void limitSphere();
    bool checkIntersection(double &rotatingArm, bool other);
    void setSphereBool(bool myBool);
    bool getSphereBool();
    float getXPos();
    float getYPos();
    void paint (juce::Graphics&) override;
    void resized() override;
    
    float x;
    float y;
    double spx;
    double spy;
    float magVecA;
    
private:
    
    float cx;
    float cy;
    Coord cp;
    Coord mp;
    Coord vecA;
    Coord rp;
    Coord nVecB;
    Coord scaledPos;
    Coord newVecA;
    float phi;
    float dist0;
    float dist1;
    float dist2;
    double scalarProjection;
    int sphereRadius { 10 };
    bool isIntersecting = true;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sphere)
};



