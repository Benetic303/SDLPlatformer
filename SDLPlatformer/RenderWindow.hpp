#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "entity.hpp"
//#include "camera.hpp"

class Camera;

class RenderWindow {
public:
	RenderWindow(const char* p_title, int p_w, int p_h);

	SDL_Texture* loadTexture(const char* p_filePath);

	int getRefreshRate();

	void cleanUp();
	void clear();
	void render(Entity& p_entity, Camera camera);
	void renderText(SDL_Texture* p_tex, float p_x, float p_y, float p_w, float p_h);
	void display();

	// Add methods for text rendering
	TTF_Font *loadFont(const char* p_filePath, int p_fontSize);
	SDL_Texture* createTextTexture(TTF_Font* p_font, const char* p_text, size_t length, SDL_Color p_color);

	int getWindowHeight() const;
	int getWindowWidth() const;

	SDL_Window* window;

private:

	SDL_Renderer* renderer;


};