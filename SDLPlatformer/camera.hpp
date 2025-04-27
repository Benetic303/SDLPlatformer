#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "math.hpp"
#include "RenderWindow.hpp"

class Camera {
public:
	Camera(Vector2f	pos);
	void update(float PlayerPosX, float PlayerPosY, int WindowWidth, int WindowHeight);


	Vector2f cameraPos;

	float zoom = 2.0f; // Default zoom level (1.0 = no zoom)

private:
};