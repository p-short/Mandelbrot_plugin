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


//myFunctions header and .cpp contain custom helper files which are used to relaise alot of the trigonometry and vector math used in the positioning of node circles and collision detection.

//Coord is a struct that contains two float variables, x & y.
struct Coord {
    float x;
    float y;
};

//declaration of methods. See .cpp for definitions.

// return coordinats
Coord createCoord(float x_, float y_);

// return vector
Coord createVector(Coord v0, Coord v1);

// return normalised vector
Coord normalise(Coord v);

// return the dotproduct
float dotProduct(Coord v0,Coord v1);

// return magnitude of vector
float magnitude(Coord v);

//returns the distance between two coords
float distance(float x0, float y0, float x1, float y1);

//clamp a value to a min and max so the value stays in that constraint.
float clampIt(float value, float min, float max);

//take a range and extend it to another.
float changeRange(float value, float a, float b, float c, float d);
