#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "entity.hpp"

class RenderWindow {
public:
	RenderWindow(const char* p_title, int p_w, int p_h);

	SDL_Texture* loadTexture(const char* p_filePath);

	int getRefreshRate();

	void cleanUp();
	void clear();
	void render(Entity& p_entity);
	void display();

	int getWindowHeight() const;
	int getWindowWidth() const;

	SDL_Window* window;

private:

	SDL_Renderer* renderer;


};