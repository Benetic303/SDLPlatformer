#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Utils.hpp"

bool checkCollision(const SDL_FRect& a, const SDL_FRect& b) {


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


    //If left side of A is the the right of B
    if (aMinX >= bMaxX)
    {
        return false;
    }

    //If the right side of A to the left of B
    if (aMaxX <= bMinX)
    {
        return false;
    }

    //If the top side of A to the bottom of B
    if (aMinY >= bMaxY) {
        return false;
    }

    //If the bottom side of A to the top side of B
    if (aMaxY <= bMinY) {
        return false;
    }

    // If none of the sides from A are outside B
    return true;
}