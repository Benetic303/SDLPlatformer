#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>

#include "Utils.hpp"
#include "World.hpp"
#include "CollisionSide.hpp"



CollisionSide checkCollision(const SDL_FRect& a, const SDL_FRect& b) {
    const float tolerance = 0.1f; // Tolerance to avoid floating-point precision issues

    //Calculate the sides of rect A
    float aMinX = a.x;
    float aMaxX = a.x + a.w;
    float aMinY = a.y;
    float aMaxY = a.y + a.h;

    //Calculate the sides of rect B
    float bMinX = b.x;
    float bMaxX = b.x + b.w;
    float bMinY = b.y;
    float bMaxY = b.y + b.h;


    // Check if there is no collision
    if (aMinX >= bMaxX - tolerance || aMaxX <= bMinX + tolerance || 
        aMinY >= bMaxY || aMaxY <= bMinY) {
        return NONE; // No collision
    }

    // Determine the side of the collision
    float overlapBottom = aMaxY - bMinY;   // Overlap if moving up
    float overlapTop = bMaxY - aMinY; // Overlap if moving down
    float overlapLeft = aMaxX - bMinX;  // Overlap if moving left
    float overlapRight = bMaxX - aMinX; // Overlap if moving right
 
	
    // Find the smallest overlap to determine the collision side
    float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

    CollisionSide result = NONE;
    if (std::abs(minOverlap - overlapLeft) < tolerance) result = (CollisionSide)(result | LEFT);
    if (std::abs(minOverlap - overlapRight) < tolerance) result = (CollisionSide)(result | RIGHT);
    if (std::abs(minOverlap - overlapTop) < tolerance) result = (CollisionSide)(result | TOP);
    if (std::abs(minOverlap - overlapBottom) < tolerance) result = (CollisionSide)(result | BOTTOM);

    return result;
}


int count_digit(int number) {
    int count = 0;
    while (number != 0) {
        number = number / 10;
        count++;
    }
    return count;
}


void moveAndCollide(World& world, SDL_FRect& aabb, float& pos, float delta, bool isVertical) {
    const float step = 4.0f; // Smaller step for more precision
    float moved = 0.0f;
    float sign = (delta > 0) ? 1.0f : -1.0f;
    while (std::abs(moved) < std::abs(delta)) {
        float move = std::min(step, std::abs(delta - moved)) * sign;
        if (isVertical) aabb.y += move;
        else aabb.x += move;

        CollisionSide side = world.CollidingWithTerrain(aabb);
        if (side != NONE) {
            // Undo the last move
            if (isVertical) aabb.y -= move;
            else aabb.x -= move;
            break;
        }
        moved += move;
    }
    if (isVertical) pos = aabb.y;
    else pos = aabb.x;
}