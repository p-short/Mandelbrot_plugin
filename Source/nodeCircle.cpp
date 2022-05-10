/*
  ==============================================================================

    Spheres.cpp
    Created: 29 Jan 2022 3:24:01pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "nodeCircle.h"
#include "myFunctions.h"

//==============================================================================
//constuctor sets private member variables x, y, cx, cy to 0
NodeCircle::NodeCircle()
{
    x = 0;
    y = 0;
    cx = 0;
    cy = 0;
}

NodeCircle::~NodeCircle()
{
}

//setPosition takes the values from the position sliders of position modulation source on the GUI
void NodeCircle::setPosition(float x_, float y_, float cx_, float cy_)
{
    x = x_;
    y = y_;
    cx = cx_;
    cy = cy_;
}

//updatePosition takes a vector full of node circles and calculates where each node circle should be
void NodeCircle::updatePosition(std::vector<std::unique_ptr<NodeCircle>>& thing)
{
    for (int i = 0; i < thing.size() - 1; i++)
    {
        //takes a look at the first node circles position squares its position then stores its value in the element next to it in the vector. This is an iterative process that happens over and over again.
        thing[i + 1]->x = (pow(thing[i]->x, 2) + -pow(thing[i]->y, 2)) +
        (pow(thing[i]->cx, 2) + -pow(thing[i]->cy, 2));
        thing[i + 1]->y = 2 * thing[i]->x * thing[i]->y + 2 * thing[i]->cx * thing[i]->cy;
    }
}

//limitPosition ensures that the nodeCircles stay inside the border circle
void NodeCircle::limitPosition()
{
    //cp = circle point
    //the centre of each node circle;
    cp = createCoord(x, y);
    //mp = middle point
    mp = createCoord(0, 0);
    //vecA is the vector between circle point & middle point
    vecA = createVector(cp, mp);
    
    //arctangent2 function returns the angle in radians of the position of nodeCircle
    phi = atan2(y, x);
    
    //due to the nature of repeatedly squaring numbers sometimes during runtime under certain configurations of the position sliders / position modulation source the nodeCircles variables will return NaN, this will crash the program. This is why I am checking values to see if they are NaN or not, if they are its likely they are at a point positionally which is way past the boundary of the border circle, so when its NaN set value to a fixed position.
    
    //check if "phi" is not a number, if it is set to two pi
    if (isnan(phi))
    {
        phi = M_PI * 2;
    }
    
    //clamp the magnitude of the vector between circle point & middle point between 0 and 1
    magVecA = clampIt(magnitude(vecA), 0 , 1);
    
    //check if "magVecA" is not a number, if it is set max value of 1
    if (isnan(magVecA))
    {
        magVecA = 1.0;
    }
    
    //set newX & newY to the correct polar coordinates, this is whats returned from the getX and getY methods
    newX = magVecA * cos(phi);
    newY = magVecA * sin(phi);
}

//checks to see if nodeCircle is tangent of intersecting with rotating arm, if it increase brightness of colour, if not add transrency. this is to light up the nodeCircle when its hit.
bool NodeCircle::checkForPaint(double inc)
{
    //scale position from 0 to 1 from 0 to border radius minus nodecircle radius so it stays within the border circle
    scaledPos = createCoord(newX * (bordRad - nodeCircleRadius), newY * (bordRad - nodeCircleRadius));
    //rp = the rotation value in radians
    rp = createCoord(bordRad * cos(inc), bordRad * sin(inc));
    //nVecA is the new scaled vector of nodeCircle to the middle point
    nVecA = createVector(scaledPos, mp); // mp = (0, 0)
    //vecB = the vector between the top point the the rotating arm and the middle point
    vecB = createVector(rp, mp);
    //normalise vecB
    nVecB = normalise(vecB);
    //calculate the scalar projection of nVecA and nVecB, this is a perpendicular line from the rotating arm to the middle of nodeCircle
    scalarProjection = clampIt(dotProduct(nVecA, nVecB), 0, bordRad - nodeCircleRadius);
    
    //store x and y position of scalar projection
    spx = scalarProjection * cos(inc);
    spy = scalarProjection * sin(inc);
    
    //work out euclidian distance between.
    //top of rotating arm and nodeCircle
    dist0 = distance(rp.x, rp.y, scaledPos.x, scaledPos.y);
    //nodeCircle and scalar projection
    dist1 = distance(scaledPos.x, scaledPos.y, spx, spy);
    //nodeCircle and middle point
    dist2 = distance(scaledPos.x, scaledPos.y, mp.x, mp.y);
    
    //whenever the distance from the scalar projection (perpendicular line from the rotating arm to the middle of nodeCircle) to nodeCircle is less than the radius of nodeCircle, return true. thus changing colour of nodeCircle in the paint method within pluginEditor.cpp
    return dist0 <= bordRad && dist1 < nodeCircleRadius && dist2 > nodeCircleRadius;
}

//return x position
double NodeCircle::getXPos()
{
    return newX;
}
//return y position
double NodeCircle::getYPos()
{
    return newY;
}

void NodeCircle::paint (juce::Graphics& g)
{
}

void NodeCircle::resized()
{
}

