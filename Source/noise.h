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

//for my project I used this noise for I needed the noises range to be between -1 & 1.
//if you need its range to be 0 to 1 just remove the setRange function and simply return "r".
auto setRange = [](float value, float a, float b, float c, float d)
{
    value = (value - a) / (b - a);
    return c + value * (d - c);
};

float *perlin = nullptr;

float myPerlinNoise(float x)
{
    if (perlin == nullptr)
    {
//        std::cout << "this should only be called once";
        perlin = new float[PERLIN_SIZE + 1];
        for (int i = 0; i < PERLIN_SIZE + 1; i++) {
            perlin[i] = (float) rand() / RAND_MAX;
        }
    }
    
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
    return setRange(r, 0, 1, -1, 1);
}

#endif /* noise_h */
