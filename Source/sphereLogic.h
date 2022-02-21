/*
  ==============================================================================

    sphereLogic.h
    Created: 19 Feb 2022 12:28:43pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "myFunctions.h"

class SphereLogic {
public:
    SphereLogic();
    ~SphereLogic();
    
    void setPosition(float _x, float _y, float _cx, float _cy);
    void updatePosition(std::vector<std::unique_ptr<SphereLogic>>&);
    void limitSphere();
    bool checkIntersection(double &rotatingArm, bool other);
    void setSphereBool(bool myBool);
    bool getSphereBool();
    float getXPos();
    float getYPos();
    
private:
    
    float x;
    float y;
    float cx;
    float cy;
    Coord centPoint;
    Coord spherePoint;
    Coord vecA;
    Coord rotationPoint;
    Coord normVecB;
    Coord scaledPos;
    Coord newVecA;
    float phi;
    float magVecA;
    float spx;
    float spy;
    float dist0;
    float dist1;
    float dist2;
    double scalarProjection;
    int sphereRadius { 10 };
    bool isIntersecting = true;
};
