#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>

#include "Utils.hpp"



CollisionSide checkCollision(const SDL_FRect& a, const SDL_FRect& b) {


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
    if (aMinX >= bMaxX || aMaxX <= bMinX || aMinY >= bMaxY || aMaxY <= bMinY) {
        return NONE; // No collision
    }

    // Determine the side of the collision
    float overlapBottom = aMaxY - bMinY;   // Overlap if moving up
    float overlapTop = bMaxY - aMinY; // Overlap if moving down
    float overlapLeft = aMaxX - bMinX;  // Overlap if moving left
    float overlapRight = bMaxX - aMinX; // Overlap if moving right
 

    // Find the smallest overlap to determine the collision side
    float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

    if (minOverlap == overlapLeft) {
        return LEFT;
    }
    else if (minOverlap == overlapRight) {
        return RIGHT;
    }
    else if (minOverlap == overlapTop) {
        return TOP;
    }
    else if (minOverlap == overlapBottom) {
        return BOTTOM;
    }

    return NONE; //Fallback
}
