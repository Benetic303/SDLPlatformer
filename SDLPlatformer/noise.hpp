#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <math.h>
#include <vector>

#include "math.hpp"



class PerlinNoise {
public:
	
	Vector2f randomGradient(int ix, int iy);
	float doGridGradient(int ix, int iy, float x, float y);


	float interpolate(float a0, float a1, float w);

	//Sample Perlin noise at coordinates x, y
	float perlin(float x, float y);
private:

};


