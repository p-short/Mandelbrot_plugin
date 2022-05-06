/*
  ==============================================================================

    noise.h
    Created: 23 Feb 2022 7:45:21pm
    Author:  Iansyst Loan

  ==============================================================================
*/

#pragma once
#ifndef noise_h
#define noise_h

#include <cmath>
#include <cstdlib>

//This header file contains the Perlin noise algorithm used to modulate the positions of the node circles when "white noise" is selected on the position modulating button.

//The Perlin noise implementation is adapted from the noise function from the JavaScript open source library p5.js https://p5js.org, to get it to run on C++ i needed to modify and translate the code.

int PERLIN_YWRAPB = 4;
int PERLIN_YWRAP = 1 << PERLIN_YWRAPB;
int PERLIN_ZWRAPB = 8;
int PERLIN_ZWRAP = 1 << PERLIN_ZWRAPB;
int PERLIN_SIZE = 4095;
int perlin_octaves = 2;
float perlin_amp_falloff = 0.5;

auto scaled_cosine = [](float i)
{
    return 0.5 * (1.0 - cos(i * M_PI));
};

//lamda function to set one range to another
auto setRange = [](float value, float a, float b, float c, float d)
{
    value = (value - a) / (b - a);
    return c + value * (d - c);
};

float *perlin = nullptr;

float myPerlinNoise(float x)
{
    //if perlin pointer is null fill array the size of PERLIN_SIZE with random values.
    if (perlin == nullptr)
    {
        perlin = new float[PERLIN_SIZE + 1];
        for (int i = 0; i < PERLIN_SIZE + 1; i++) {
            perlin[i] = (float) rand() / RAND_MAX;
        }
    }
    //process array and smooth values to produce Perlin noise.
    if (x < 0)
    {
        x = -x;
    }
    
    int xi = floor(x);
    float xf = x - xi;
    float rxf;
    
    float r = 0;
    float ampl = 0.5;
    float n1, n2, n3;
    
    for (int o = 0; o < perlin_octaves; o++)
    {
        int of = xi;
        
        rxf = scaled_cosine(xf);
        
        n1 = perlin[of & PERLIN_SIZE];
        n1 += rxf * (perlin[(of + 1) & PERLIN_SIZE] - n1);
        n2 = perlin[(of + PERLIN_YWRAP) & PERLIN_SIZE];
        n2 += rxf * (perlin[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n2);
        n1 += (n2 - n1);
        
        of += PERLIN_ZWRAP;
        n2 = perlin[of & PERLIN_SIZE];
        n2 += rxf * (perlin[(of + 1) & PERLIN_SIZE] - n2);
        n3 = perlin[(of + PERLIN_YWRAP) & PERLIN_SIZE];
        n3 += rxf * (perlin[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n3);
        n2 += (n3 - n2);
        
        n1 += (n2 - n1);
        
        r += n1 * ampl;
        ampl *= perlin_amp_falloff;
        xi <<= 1;
        xf *= 2;
        
        if (xf >= 1.0)
        {
            xi++;
            xf--;
        }
    }
    //return values with the range of -1 to 1
    return setRange(r, 0, 1, -1, 1);
}

#endif /* noise_h */
