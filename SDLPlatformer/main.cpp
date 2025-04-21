#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "entity.hpp"
#include "Utils.hpp"
#include "Player.hpp"




int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cout << "Video Initialisation Error: " << SDL_GetError() << std::endl;

	/*if ((!IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;*/

	RenderWindow window("Game v1.0", 1280, 720);
	bool fullscreen = false;

	
	SDL_Texture* grassTexture = window.loadTexture("assets/textures/Graphics/ground_grass_1.png");

	SDL_Texture* playerTexture = window.loadTexture("assets/textures/Graphics/hulking_knight - Kopie.png");


	std::vector<Entity> entities = {};

	for (int i = 0; i <= 2; i++) {
		entities.emplace_back(Entity({i * 32.0f, (float) window.getWindowHeight()/6 - 32 }, grassTexture, 32, 32));
	}

	for (int i = 0; i <= 4; i++) {
		entities.emplace_back(Entity({ 2*i * 32.0f, (float)window.getWindowHeight()/6 - 32 }, grassTexture, 32, 32));
	}
	for (int i = 0; i <= 5; i++) {
		entities.emplace_back(Entity({5 * i * 32.0f, (float)window.getWindowHeight() / 3 - 32 }, grassTexture, 32, 32));
	}
	for (int i = 0; i <= 15; i++) {
		entities.emplace_back(Entity({i * 32.0f, (float)window.getWindowHeight()/2 - 32 }, grassTexture, 32, 32));
	}

	std::cout << window.getWindowHeight() << std::endl;

	
	Player player({ 50, 50 }, playerTexture, 30, 46);

	std::vector<bool> keyStates(SDLK_Z, false);
	


	bool isRunning = true;
	SDL_Event event;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();

	while (isRunning) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				isRunning = false;
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_F11) {
				fullscreen = !fullscreen;
				SDL_SetWindowFullscreen(window.window, (fullscreen ? true : false));
			}
			

	
	


			if (event.type == SDL_EVENT_KEY_UP) {
				if (event.key.key >= 0 && event.key.key < SDLK_Z) {
					keyStates[event.key.key] = false;
				}
			}
			if (event.type == SDL_EVENT_KEY_DOWN) {

				if (event.key.key >= 0 && event.key.key < SDLK_Z) {
					keyStates[event.key.key] = true;
				}
			}
		}


		int startTicks = SDL_GetTicks();

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		currentTime = newTime;
		accumulator += frameTime;

		

		while (accumulator >= timeStep) {

			player.update(timeStep, keyStates, entities);


			accumulator -= timeStep;

			

		}

	


		
		// --- End of Game State Update ---


		// Calculate interpolation factor for smooth rendering between physics steps
		const float alpha = accumulator / timeStep;


		window.clear();

		for (Entity& e : entities) {
			window.render(e);
		}
		window.render(player);

		window.display();


		


		int frameTicks = SDL_GetTicks() - startTicks;
		if (window.getRefreshRate() > 0) {
			const int targetFrameTime = 1000 / window.getRefreshRate();
			if (frameTicks < targetFrameTime) {
				SDL_Delay(targetFrameTime - frameTicks);
			}
		}
	}



	window.cleanUp();
	SDL_Quit();

	return 0;
}