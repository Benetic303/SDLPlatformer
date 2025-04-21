#pragma once

#include <SDL3/SDL.h>


namespace utils {
	inline float hireTimeInSeconds() {
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}
}

bool checkCollision(const SDL_FRect& a, const SDL_FRect& b);