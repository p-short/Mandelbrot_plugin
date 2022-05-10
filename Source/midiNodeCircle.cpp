/*
  ==============================================================================

    sphereLogic.cpp
    Created: 19 Feb 2022 12:28:43pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#include "midiNodeCircle.h"

//constructor inits x, y, cx, cy variables to 0
MidiNodeCircle::MidiNodeCircle()
{
    x = 0;
    y = 0;
    cx = 0;
    cy = 0;
}

MidiNodeCircle::~MidiNodeCircle()
{
    
}

//set position of midiNodeCircle, the values from the GUI thread using the sliders are passed to the audio Thread and used in this class
void MidiNodeCircle::setPosition(float _x, float _y, float _cx, float _cy)
{
    x = _x;
    y = _y;
    cx = _cx;
    cy = _cy;
}
//update the position of the midiNodeCircle
void MidiNodeCircle::updatePosition(std::vector<std::unique_ptr<MidiNodeCircle> > &myVector)
{
    for (int i = 0; i < myVector.size() -1 ; i++)
    {
        //takes a look at the first node circles position squares its position then stores its value in the element next to it in the vector. This is an iterative process that happens over and over again.
        myVector[i + 1]->x = (pow(myVector[i]->x, 2) + -pow(myVector[i]->y, 2)) +
        (pow(myVector[i]->cx, 2) + -pow(myVector[i]->cy, 2));
        myVector[i + 1]->y = 2 * myVector[i]->x * myVector[i]->y + 2 * myVector[i]->cx * myVector[i]->cy;
    }
}

//limit the position of the midiNodeCircle so it cannot leave the border circle
void MidiNodeCircle::limitNodeCircle()
{
    //ap_cp = circle point
    //the centre of each node circle;
    ap_cp = createCoord(x, y);
    //ap_mp = middle point
    ap_mp = createCoord(0, 0);
    //ap_vecA is the vector between circle point & middle point
    ap_vecA = createVector(ap_cp, ap_mp);
    
    //if any of the MidiNodeCircle variables return NaN set to fixed value as to note throw an error and crash the program
    ap_phi = atan2(y, x);
    
    if (isnan(ap_phi))
    {
        ap_phi = M_PI * 2;
    }
    
    ap_magVecA = clampIt(magnitude(ap_vecA), 0 , 1);
    
    if (isnan(ap_magVecA))
    {
        ap_magVecA = 1.0;
    }
    
    //polar coordinates
    ap_newX = ap_magVecA * cos(ap_phi);
    ap_newY = ap_magVecA * sin(ap_phi);
}

bool MidiNodeCircle::checkIntersection(double &rotatingArm, bool other)
{
    //this is largely the same code as the NodeCircle classes check intersection. its calculated the scalar projected of the rotating arm on to a midiNodeCircle. when the distance of the scalar projection is less than the midiNodeCircles radius the rotating arm is intersecting withmidiNodeCircle.
    ap_scaledPos = createCoord(ap_newX * (ap_bordRad - ap_sphereRadius),
                               ap_newY * (ap_bordRad - ap_sphereRadius));
    
    ap_rp = createCoord(ap_bordRad * cos(rotatingArm), ap_bordRad * sin(rotatingArm));
    
    ap_newVecA = createVector(ap_scaledPos, ap_mp);
    ap_nVecB = normalise(createVector(ap_rp, ap_mp));
    
    ap_scalarProjection = clampIt(dotProduct(ap_newVecA, ap_nVecB), 0, ap_bordRad - ap_sphereRadius);
    
    ap_spx = ap_scalarProjection * cos(rotatingArm);
    ap_spy = ap_scalarProjection * sin(rotatingArm);
    
    //work out euclidian distance between.
    //top of rotating arm and midiNodeCircle
    ap_dist0 = distance(ap_rp.x, ap_rp.y, ap_scaledPos.x, ap_scaledPos.y);
    //midiNodeCircle and scalar projection
    ap_dist1 = distance(ap_scaledPos.x, ap_scaledPos.y, ap_spx, ap_spy);
    //midiNodeCircle and middle point
    ap_dist2 = distance(ap_scaledPos.x, ap_scaledPos.y, ap_mp.x, ap_mp.y);
    
    //when collision is detected  return true to trigger midinote, then reset the appropriate midiNodeCircles internal bool to false so this only happens once per collision.
    if (ap_dist0 < ap_bordRad && ap_dist1 < ap_sphereRadius && other && ap_dist2 > ap_sphereRadius)
    {
        return true;
    }
    
    else if (ap_dist0 < ap_bordRad && ap_dist1 > ap_sphereRadius)
    {
        ap_isIntersecting = true;
        return false;
    }
    
    else
    {
        return false;
    }
}

//a way of externally setting individual instances to true / false.
void MidiNodeCircle::setSphereBool(bool myBool)
{
    ap_isIntersecting = myBool;
}

//retun bool
bool MidiNodeCircle::getSphereBool()
{
    return ap_isIntersecting;
}

//return x position
float MidiNodeCircle::getXPos()
{
    return ap_newX;
}

//return y position
float MidiNodeCircle::getYPos()
{
    return ap_newY;
}

//return length of vector between the middile of the boundy circle and a midiNodeCircle, this is used to set the midinotes velocity when the velocity button is on.
float MidiNodeCircle::getMag()
{
    return ap_magVecA;
}
