#pragma once

#include <SDL3/SDL.h>
#include "CollisionSide.hpp"

//Forward declaration, to break circular dependency with "World.hpp"
class World;

namespace utils {
	inline float hireTimeInSeconds() {
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}
}

CollisionSide checkCollision(const SDL_FRect& a, const SDL_FRect& b);

int count_digit(int number);				 

void moveAndCollide(World& world, SDL_FRect& aabb, float& pos, float delta, bool isVertical);