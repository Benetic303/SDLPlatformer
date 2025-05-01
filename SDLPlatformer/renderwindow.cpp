#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <SDL3/SDL_render.h>

#include "RenderWindow.hpp"
#include "entity.hpp"
#include "camera.hpp"

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
void RenderWindow::render(Entity& p_entity, Camera camera) {
    Vector2f entityPos = p_entity.getPos();

    SDL_FRect src = p_entity.getCurrentFrame();

    SDL_FRect dst;
    dst.x = (entityPos.x - camera.cameraPos.x) * camera.zoom; // Adjust position relative to the camera
    dst.y = (entityPos.y - camera.cameraPos.y) * camera.zoom; // Adjust position relative to the camera
    dst.w = src.w * camera.zoom; // Width of the entity
    dst.h = src.h * camera.zoom; // Height of the entity

    // Render the entity's texture
    SDL_RenderTexture(renderer, p_entity.getTex(), &src, &dst);
}

void RenderWindow::renderText(SDL_Texture* p_tex, float p_x, float p_y, float p_w, float p_h) {
    if (!renderer || !p_tex) {
        std::cout << "Error: Renderer or Texture is NULL in RenderWindow::render(text)." << std::endl;
        return;
    }

    // Source rectangle: use the whole texture
    SDL_FRect src = { 0.0f, 0.0f, p_w, p_h }; // Assuming p_w and p_h are texture dimensions

    // Destination rectangle: use the provided screen coordinates
    SDL_FRect dst = { p_x, p_y, p_w, p_h };

    SDL_RenderTexture(renderer, p_tex, &src, &dst);
}


void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

// Implementation of loadFont
TTF_Font *RenderWindow::loadFont(const char* p_filePath, int p_fontSize) {
    TTF_Font* font = TTF_OpenFont(p_filePath, p_fontSize);
    return font;
}

// Implementation of createTextTexture
SDL_Texture *RenderWindow::createTextTexture(TTF_Font* p_font, const char* p_text, size_t length, SDL_Color p_color) {
    if (!renderer || !p_font || !p_text) {
        std::cout << "Error: Invalid parameters for createTextTexture." << std::endl;
        return NULL;
    }

    // Render the text to a surface (Solid is fastest, but no anti-aliasing)
    // Use TTF_RenderText_Blended for anti-aliased text
    SDL_Surface* textSurface = TTF_RenderText_Solid(p_font, p_text, length, p_color);
    if (textSurface == NULL) {
        return NULL;
    }

    // Create a texture from the surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        std::cout << "Failed to create texture from text surface. SDL_Error: " << SDL_GetError() << std::endl;
    }

    // Free the surface (we no longer need it after creating the texture)
    SDL_DestroySurface(textSurface);

    return textTexture;
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