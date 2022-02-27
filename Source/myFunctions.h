/*
  ==============================================================================

    myFunctions.h
    Created: 2 Feb 2022 11:30:39pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#pragma once
#include <cmath>
#include <algorithm>


// function prototypes

struct Coord {
    float x;
    float y;
};

// return coordinats
Coord createCoord(float x_, float y_);

// return vector
Coord createVector(Coord v0, Coord v1);

// return the dotproduct
float dotProduct(Coord v0,Coord v1);

// return magnitude of vector
float magnitude(Coord v);

// return normalised vector
Coord normalise(Coord v);

//returns the distance between two coords
float distance(float x0, float y0, float x1, float y1);

//clamp a value to a min and max so the value stays in that constraint.
float clampIt(float value, float min, float max);

//take a range and extend it to what you want.
float changeRange(float value, float a, float b, float c, float d);
