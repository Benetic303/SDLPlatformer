#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "math.hpp"

class Entity {
public:
	Entity(Vector2f p_pos, SDL_Texture* p_tex, int width, int height);
	Vector2f& getPos() {
		return pos;
	}
	SDL_Texture* getTex();
	SDL_FRect getCurrentFrame();


	
protected: //protected means derived classes (like Player) can access pos
	Vector2f pos;
	SDL_FRect currentFrame;
	
	
private:

	SDL_Texture* tex;

};