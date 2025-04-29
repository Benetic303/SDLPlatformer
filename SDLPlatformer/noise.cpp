#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <math.h>

#include "noise.hpp"




Vector2f PerlinNoise::randomGradient(int ix, int iy) {
	//No precomputed gradients mean this works for any number of grid coordinates
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	a *= 3284157443;

	b ^= a << s | a >> w - s;
	b *= 1911520717;

	a ^= b << s | b >> w - s;
	a *= 2048419325;
	float random = a * (3.14159265 / ~(~0u >> 1)); //in [0, 2*pi]

	//Create the vector from the angle
	Vector2f v;
	v.x = sin(random);
	v.y = cos(random);
	

	return v;
}

//Computes the dot product of the distance and gradient vectors
float PerlinNoise::doGridGradient(int ix, int iy, float x, float y) {
	//Get gradient from integer coordinates
	Vector2f gradient = randomGradient(ix, iy);

	//Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	//Compute the dot-product
	return (dx * gradient.x + dy * gradient.y);

}


float PerlinNoise::interpolate(float a0, float a1, float w) {
	return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
}




float PerlinNoise::perlin(float x, float y) {

	// Determine grid cell corner coordinates
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	//Compute Interpolation weights
	float sx = x - (float)x0;
	float sy = y - (float)y0;


	//Compute and interpolate top two corners
	float n0 = doGridGradient(x0, y0, x, y);
	float n1 = doGridGradient(x1, y0, x, y);
	float ix0 = interpolate(n0, n1, sx);


	//Compute and interpolate bottom two corners
	n0 = doGridGradient(x0, y1, x, y);
	n1 = doGridGradient(x1, y1, x, y);
	float ix1 = interpolate(n0, n1, sx);

	float value = interpolate(ix0, ix1, sy);

	return value;
}

