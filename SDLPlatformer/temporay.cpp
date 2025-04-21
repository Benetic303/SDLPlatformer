//#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
//#include <SDL3_image/SDL_image.h>
//#include <iostream>
//
//int main(int argc, char* argv[]) {
//	SDL_Window* window = nullptr;
//	SDL_Surface* windowSurface = nullptr;
//	SDL_Surface* image1 = nullptr;
//	SDL_Surface* image2 = nullptr;
//	SDL_Surface* image3 = nullptr;
//	SDL_Surface* currentImage = nullptr;
//
//	if (SDL_Init(SDL_INIT_VIDEO) < 0)
//		std::cout << "Video Initialisation Error: " << SDL_GetError() << std::endl;
//	else {
//		window = SDL_CreateWindow("SDLPlatformer", 640, 480, SDL_WINDOW_RESIZABLE);
//		if (window == NULL)
//			std::cout << "Window creation Error: " << SDL_GetError() << std::endl;
//	}
//	windowSurface = SDL_GetWindowSurface(window);
//
//	image1 = SDL_LoadBMP("assets/textures/w3c_home.bmp");
//	image2 = SDL_LoadBMP("assets/textures/PixelArtTutorial.png");
//	image3 = SDL_LoadBMP("assets/textures/nhq202211160203.bmp");
//	if (image1 == NULL || image2 == NULL || image3 == NULL)
//		std::cout << "Loading Image Error: " << SDL_GetError() << std::endl;
//	currentImage = image1;
//
//	bool isRunning = true;
//	SDL_Event ev;
//
//	while (isRunning) {
//		while (SDL_PollEvent(&ev) != 0) {
//			if (ev.type == SDL_EVENT_QUIT)
//				isRunning = false;
//			else if (ev.type == SDL_EVENT_KEY_DOWN) {
//				switch (ev.key.key) {
//				case SDLK_1:
//					currentImage = image1;
//					break;
//				case SDLK_2:
//					currentImage = image2;
//					break;
//				case SDLK_3:
//					currentImage = image3;
//					break;
//				}
//			}
//		}
//		SDL_BlitSurface(currentImage, NULL, windowSurface, NULL);
//
//		SDL_UpdateWindowSurface(window);
//
//	}
//	SDL_DestroySurface(image1);
//	SDL_DestroySurface(image2);
//	SDL_DestroySurface(image3);
//
//	SDL_DestroyWindow(window);
//
//	currentImage = image1 = image2 = image3 = nullptr;
//	window = nullptr;
//
//	SDL_Quit();
//
//	return 0;
//}