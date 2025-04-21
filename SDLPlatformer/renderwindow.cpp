#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <SDL3/SDL_render.h>

#include "RenderWindow.hpp"
#include "entity.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, p_w, p_h, SDL_WINDOW_RESIZABLE);

	if (window == NULL)
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;

	renderer = SDL_CreateRenderer(window, NULL);
}



SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}

int RenderWindow::getRefreshRate() {
    // Check if the provided window is valid
    if (!window) {
        std::cerr << "Error: Invalid window provided to getMonitorRefreshRate." << std::endl;
        return 0.0f; // Indicate failure
    }

    // Get the ID of the display that the window is currently on
    SDL_DisplayID displayId = SDL_GetDisplayForWindow(window);
    if (displayId == 0) {
        std::cerr << "Error getting display for window: " << SDL_GetError() << std::endl;
        return 0.0f; // Indicate failure
    }

    // Get the current desktop display mode for the identified display.
    const SDL_DisplayMode* desktopMode = SDL_GetDesktopDisplayMode(displayId);
    if (!desktopMode) { // Check if the returned mode is valid
        std::cerr << "Error getting desktop display mode: " << SDL_GetError() << std::endl;
        return 0.0f; // Indicate failure
    }

    
    // refresh rate is a direct member of SDL_DisplayMode.
    float refreshRate = desktopMode->refresh_rate;

    // Note: desktopMode does NOT need to be freed/destroyed when obtained via SDL_GetDesktopDisplayMode.

    // Check if the retrieved refresh rate is valid (greater than 0).
    // A value of 0.0f might indicate an error or genuinely unspecified refresh rate.
    if (refreshRate <= 0.0f) {
        std::cerr << "Warning: Could not retrieve a valid refresh rate (received " << refreshRate << "). Display mode might be invalid or refresh rate is unspecified." << std::endl;
    }

    return refreshRate;
}


void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}
void RenderWindow::clear() {
	SDL_RenderClear(renderer);
}
void RenderWindow::render(Entity& p_entity) {
	//SDL_Rect 
	SDL_FRect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_FRect dst;
	dst.x = p_entity.getPos().x * 2;
	dst.y = p_entity.getPos().y * 2;
	dst.w = p_entity.getCurrentFrame().w * 2;
	dst.h = p_entity.getCurrentFrame().h * 2;

   

	SDL_RenderTexture(renderer, p_entity.getTex(), &src, &dst);
}


void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

// Implementation of getWindowHeight
int RenderWindow::getWindowHeight() const {
    if (window) {
        int w, h;
        SDL_GetWindowSize(window, &w, &h); // Get both width and height
        return h; // Return the height
    }
    return 0; // Return 0 if window is not valid
}

// Implementation of getWindowWidth (if you added it)
int RenderWindow::getWindowWidth() const {
    if (window) {
        int w, h;
        SDL_GetWindowSize(window, &w, &h); // Get both width and height
        return w; // Return the width
    }
    return 0; // Return 0 if window is not valid
}