/*
  ==============================================================================

    myFunctions.cpp
    Created: 2 Feb 2022 11:30:39pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#include "myFunctions.h"



//createCoord creates a Coord instance then instanciates the x & y variables with the value of the 2 arguments that are passed as parameters. createCoord then returns the struct with instanciated variables.
Coord createCoord(float x_, float y_)
{
    Coord coord_instance;
    coord_instance.x = x_;
    coord_instance.y = y_;
    return coord_instance;
}
// createVector creates a Coord instance then subtracts the x & y values of the 2 Coord objects passed as parameters. createVector then returns new Coord instance which x & y values hold vectors between 2 Coords.
Coord createVector(Coord v0, Coord v1)
{
    Coord coord_instance;
    coord_instance.x = v1.x - v0.x;
    coord_instance.y = v1.y - v0.y;
    return coord_instance;
}

// dotProduct takes 2 objects and returns the dot product
float dotProduct(Coord v0,Coord v1) {
    return v0.x * v1.x + v0.y * v1.y;
}

// return magnitude of vector
float magnitude(Coord v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

//normalise takes a Coord object then return the normalised vector
Coord normalise(Coord v)
{
    float m = magnitude(v);
    Coord coord_instance;
    coord_instance.x = v.x / m;
    coord_instance.y = v.y / m;
    return coord_instance;
}

//distance returns the Euclidean distance between two coords
float distance(float x0 ,float y0 , float x1, float y1)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    return sqrt(dx * dx + dy * dy);
}

//clampIt clamps a value to a min and max so the value stays in that constraint.
float clampIt(float value, float min, float max)
{
    return std::min(std::max(value, min), max);
}

//changeRange takes a value and a range then returns them values mapped to a new range
float changeRange(float value, float a, float b, float c, float d)
{
    value = (value - a) / (b - a);
    return c + value * (d - c);
};
