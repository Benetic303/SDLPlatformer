#include "entity.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

Entity::Entity(Vector2f p_pos, SDL_Texture* p_tex, int width, int height) 
	:pos(p_pos), tex(p_tex)		//Same as x = p_x;... but faster
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = width;
	currentFrame.h = height;

}

SDL_Texture* Entity::getTex() {
	return tex;
}

SDL_FRect Entity::getCurrentFrame() {
	return currentFrame;
}

