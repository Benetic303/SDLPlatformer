#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "camera.hpp"
#include "math.hpp"
#include "RenderWindow.hpp"


Camera::Camera(Vector2f pos) {

	cameraPos = pos;

}

void Camera::update(float PlayerPosX, float PlayerPosY, int WindowHeight, int WindowWidth) {
	float horizontalOffset = WindowWidth / 8; // how much to offset camera horizontally
	float verticalOffset = WindowHeight / 2.8f; // how much to offset camera vertically


    float targetX = PlayerPosX - (WindowWidth / 2.0f) + horizontalOffset;
    float targetY = PlayerPosY - (WindowHeight / 2.0f) + verticalOffset;

    // Interpolation factor (adjust for smoothness)
    float lerpFactor = 0.08f; //between 0 and 1, 0 smooth, 1 instant

    // Smoothly interpolate the camera position toward the target
    cameraPos.x += (targetX - cameraPos.x) * lerpFactor;
    cameraPos.y += (targetY - cameraPos.y) * lerpFactor;

    // Optional: Clamp the camera position to the bounds of the game world
    int worldWidth = 100000;  // Example world width
    int worldHeight = 100000; // Example world height

    if (cameraPos.x < 0) cameraPos.x = 0;
    if (cameraPos.y < 0) cameraPos.y = 0;
    if (cameraPos.x > worldWidth - WindowWidth) cameraPos.x = worldWidth - WindowWidth;
    if (cameraPos.y > worldHeight - WindowHeight) cameraPos.y = worldHeight - WindowHeight;

    
}
