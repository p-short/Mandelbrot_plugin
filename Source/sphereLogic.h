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
    float getMag();
    
private:
    
    float x;
    float y;
    float cx;
    float cy;
    Coord ap_mp; //mid point
    Coord ap_cp; // spherepoint
    Coord ap_vecA;
    Coord ap_rp; //rotation point
    Coord ap_nVecB;
    Coord ap_scaledPos;
    Coord ap_newVecA;
    float ap_phi;
    float ap_magVecA;
    float ap_newX { 0 };
    float ap_newY { 0 };
    float ap_spx;
    float ap_spy;
    float ap_dist0;
    float ap_dist1;
    float ap_dist2;
    double ap_scalarProjection;
    const int ap_sphereRadius { 10 };
    const int ap_bordRad = 170;
    bool ap_isIntersecting = true;
};
