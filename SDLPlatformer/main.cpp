#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <SDL3_ttf/SDL_ttf.h>
#include <thread>

#include "RenderWindow.hpp"
#include "entity.hpp"
#include "Utils.hpp"
#include "Player.hpp"
#include "enemy.hpp"
#include "camera.hpp"
#include "noise.hpp"
#include "Chunk.hpp"
#include "World.hpp"





int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cout << "Video Initialisation Error: " << SDL_GetError() << std::endl;

	/*if ((!IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;*/

	TTF_Init();



	RenderWindow window("Game v1.0", 1280, 720);
	bool fullscreen = false;

	//Create the world
	World world;
	world.startChunkGenerationThread();

	TTF_Font* gameFont = window.loadFont("assets/fonts/Orbitron/Orbitron-VariableFont_wght.ttf", 30);
	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Texture* scoreTextTexture = NULL;
	float scoreTextWidth = 0.0f; // Use float for texture dimensions
	float scoreTextHeight = 0.0f; // Use float for texture dimensions

	int lastDisplayedScore = -1; // Variable to track the score last rendered in the texture




	SDL_Texture* grassTexture = window.loadTexture("assets/textures/Graphics/ground_grass_1.png");

	SDL_Texture* playerTexture = window.loadTexture("assets/textures/Graphics/hulking_knight - Kopie.png");



	
	Player player({ 50, 50 }, playerTexture, 30, 46);
	std::vector<bool> keyStates(SDL_SCANCODE_COUNT, false);

	std::vector<Enemy> enemies = {};


	{
		Enemy enemy({ 100, 200 }, playerTexture, 30, 46);
		enemies.emplace_back(enemy);
	}
	Camera camera({ player.getPos().x, player.getPos().y });

	bool isRunning = true;
	SDL_Event event;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();



	PerlinNoise noiseGenerator; // Create an instance of your noise generator
	noiseGenerator.seed = 12345678910;


	const int chunkSize = 16 * 32;
	// Load initial chunks around the player's starting position
	Vector2f playerStartPos(50, 50);

	Vector2f initialChunkCoords(
		std::floor(playerStartPos.x / chunkSize) * chunkSize, 
		std::floor(playerStartPos.y / chunkSize) * chunkSize
	);


	float Noisescale = 0.001f;
	float Noisethreshold = 0.01f;

	// Load the initial chunk
	world.loadChunk(initialChunkCoords, noiseGenerator, Noisescale, Noisethreshold);
	
	Vector2f lastPlayerChunkCoords = initialChunkCoords;
	int chunkLoadCooldown = 0; // Cooldown timer

	while (isRunning) {



		//load the chunks
		Vector2f playerChunkCoords(
			std::floor(player.getPos().x / chunkSize) * chunkSize,
			std::floor(player.getPos().y / chunkSize) * chunkSize
		);

		if (chunkLoadCooldown <= 0) {
			// Load chunks if the player has moved to a new chunk
			if (playerChunkCoords.y != lastPlayerChunkCoords.y || playerChunkCoords.x != lastPlayerChunkCoords.x) {
				for (int dy = -10; dy <= 10; ++dy) {
					for (int dx = -10; dx <= 10; ++dx) {
						Vector2f chunkCoords(
							playerChunkCoords.x + dx * chunkSize,
							playerChunkCoords.y + dy * chunkSize
						);
						world.enqueueChunk(chunkCoords, noiseGenerator, Noisescale, Noisethreshold);
					}
				}
				lastPlayerChunkCoords = playerChunkCoords;
			}
			chunkLoadCooldown = 1; // Reset cooldown (e.g., 10 frames)
		}
		else {
			chunkLoadCooldown--;
		}
		


		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				isRunning = false;
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_F11) {
				fullscreen = !fullscreen;
				SDL_SetWindowFullscreen(window.window, (fullscreen ? true : false));
			}
			

	
	


			if (event.type == SDL_EVENT_KEY_UP) {
				if (event.key.scancode >= 0 && event.key.scancode < SDL_SCANCODE_COUNT) {
					keyStates[event.key.scancode] = false;
				}
			}
			if (event.type == SDL_EVENT_KEY_DOWN) {

				if (event.key.scancode >= 0 && event.key.scancode < SDL_SCANCODE_COUNT) {
					keyStates[event.key.scancode] = true;
				}
			}
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_C) {
				camera.cameraView = !camera.cameraView;
				if (camera.cameraView) {
					// Initialize freeCamPos to the current camera position
					camera.freeCamPos = camera.cameraPos;
				}
			}
		}




		int startTicks = SDL_GetTicks();

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		currentTime = newTime;
		accumulator += frameTime;

		

		while (accumulator >= timeStep) {

			



			if (camera.cameraView) {
				if (keyStates[SDL_SCANCODE_UP]) {
					camera.cameraPos.y -= 1000 * timeStep;
				}
				if (keyStates[SDL_SCANCODE_DOWN]) {
					camera.cameraPos.y += 1000 * timeStep;
				}
				if (keyStates[SDL_SCANCODE_RIGHT]) {
					camera.cameraPos.x += 1000 * timeStep;
				}
				if (keyStates[SDL_SCANCODE_LEFT]) {
					camera.cameraPos.x -= 1000 * timeStep;
				}
			}
			
			camera.update((camera.cameraView ? camera.freeCamPos.x : player.getPos().x), 
						  (camera.cameraView ? camera.freeCamPos.y : player.getPos().y), 
						  window.getWindowWidth(), 
						  window.getWindowHeight(), 
						  keyStates);
			

			for (Enemy& e : enemies) {
				e.update(timeStep, keyStates, world, player);
			}

			player.update(timeStep, keyStates, world, enemies);

		

			accumulator -= timeStep;



		}

		// --- Update Text Texture if Score Changed ---
			// Assuming player.getScore() returns the current score
		int currentScore = player.score; // Or access player.score if public

		if (currentScore != lastDisplayedScore) {
			// Score has changed, re-create the text texture

			// Destroy the old texture if it exists
			if (scoreTextTexture) {
				SDL_DestroyTexture(scoreTextTexture);
				scoreTextTexture = NULL; // Set to NULL to avoid using a dangling pointer
			}

			// Format the new score string
			std::string scoreString = "Score: " + std::to_string(currentScore);


			int scoreLength = count_digit(currentScore) + 7;
			// Create the new text texture
			if (gameFont) { // Only create if the font is loaded
				scoreTextTexture = window.createTextTexture(gameFont, scoreString.c_str(), scoreLength, textColor);
				if (scoreTextTexture) {
					// Get the dimensions of the new text texture
					SDL_GetTextureSize(scoreTextTexture, &scoreTextWidth, &scoreTextHeight);
				}
				else {
					std::cerr << "Warning: Failed to re-create score text texture." << std::endl;
				}
			}
		}

			// Update the last displayed score
			lastDisplayedScore = currentScore;


		
		// --- End of Game State Update ---


		// Calculate interpolation factor for smooth rendering between physics steps
		const float alpha = accumulator / timeStep;


		window.clear();

		// Calculate the camera's visible area
		float cameraLeft = camera.cameraPos.x;
		float cameraRight = camera.cameraPos.x + window.getWindowWidth();
		float cameraTop = camera.cameraPos.y;
		float cameraBottom = camera.cameraPos.y + window.getWindowHeight();

		// Render only chunks within the camera's view
		for (const auto& [coords, chunk] : world.getChunks()) {
			if (coords.x + chunkSize * 16 < cameraLeft || coords.x > cameraRight ||
				coords.y + chunkSize * 16 < cameraTop || coords.y > cameraBottom) {
				continue; // Skip chunks outside the camera's view
			}
			chunk.renderChunk(window.getRenderer(), camera, grassTexture);
		}


		window.render(player, camera);


		for (Enemy& enemy : enemies) {
			window.render(enemy, camera);
		}

		if (scoreTextTexture) { // Only render if the texture was created successfully
			// Example: Render score text in the top-left corner
			float textX = 10.0f; // X position on screen
			float textY = 10.0f; // Y position on screen
			// Use the actual texture dimensions for rendering
			window.renderText(scoreTextTexture, textX, textY, scoreTextWidth, scoreTextHeight);
		}


		window.display();


		


		int frameTicks = SDL_GetTicks() - startTicks;
		if (window.getRefreshRate() > 0) {
			const int targetFrameTime = 1000 / window.getRefreshRate();
			if (frameTicks < targetFrameTime) {
				SDL_Delay(targetFrameTime - frameTicks);
			}
		}
	}
	world.stopChunkGenerationThread();

	window.cleanUp();
	TTF_Quit();
	SDL_Quit();

	return 0;
}