/*
  ==============================================================================

    myFunctions.cpp
    Created: 2 Feb 2022 11:30:39pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#include "myFunctions.h"
//helper functions for trig stuff later on down the road

// return coordinats
Coord createCoord(float x_, float y_)
{
    Coord coord_instance;
    coord_instance.x = x_;
    coord_instance.y = y_;
    return coord_instance;
}
// return vector
Coord createVector(Coord v0, Coord v1)
{
    Coord coord_instance;
    coord_instance.x = v1.x - v0.x;
    coord_instance.y = v1.y - v0.y;
    return coord_instance;
}

// return the dotproduct
float dotProduct(Coord v0,Coord v1) {
    return v0.x * v1.x + v0.y * v1.y;
}

// return magnitude of vector
float magnitude(Coord v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

// return normalised vector
Coord normalise(Coord v)
{
    float m = magnitude(v);
    Coord coord_instance;
    coord_instance.x = v.x / m;
    coord_instance.y = v.y / m;
    return coord_instance;
}

//returns the distance between two coords
float distance(float x0 ,float y0 , float x1, float y1)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    return sqrt(dx * dx + dy * dy);
}

//clamp a value to a min and max so the value stays in that constraint.
float clampIt(float value, float min, float max)
{
    return std::min(std::max(value, min), max);
}
