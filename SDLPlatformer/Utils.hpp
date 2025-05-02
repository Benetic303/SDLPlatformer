#pragma once

#include <SDL3/SDL.h>

enum CollisionSide {
	NONE,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};

namespace utils {
	inline float hireTimeInSeconds() {
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}
}

CollisionSide checkCollision(const SDL_FRect& a, const SDL_FRect& b);

int count_digit(int number);

