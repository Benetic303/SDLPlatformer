#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

#include "Chunk.hpp"
#include "math.hpp"
#include "noise.hpp"



void Chunk::generateTerrain(PerlinNoise& noiseGenerator, float scale, float threshold) {
	Vector2f playerSpawn = { 50.0f, 50.0f }; // Adjust to real spawn position, with function arguments doesn't work for now    
    std::vector<std::vector<float>> noiseCache(tileData.size(), std::vector<float>(tileData[0].size()));

    for (size_t y = 0; y < tileData.size(); ++y) {
        for (size_t x = 0; x < tileData[y].size(); ++x) {
            float worldX = ChunkCoords.x + x * 32.0f;
            float worldY = ChunkCoords.y + y * 32.0f;

            // Cache the noise value
            noiseCache[y][x] = noiseGenerator.perlin(worldX * scale, worldY * scale);
        }
    }

    for (size_t y = 0; y < tileData.size(); ++y) {
        for (size_t x = 0; x < tileData[y].size(); ++x) {

            float worldX = ChunkCoords.x + x * 32.0f;
            float worldY = ChunkCoords.y + y * 32.0f;

            // Clear blocks around the player's spawn position
            float distanceToSpawn = std::sqrt(std::pow(worldX + playerSpawn.x, 2) + std::pow(worldY + playerSpawn.y, 2));
            if (distanceToSpawn < 512.0f) { // Clear a 512-pixel radius around the spawn
                tileData[y][x] = 0; // Empty tile
            }
            else {
                tileData[y][x] = (noiseCache[y][x] > threshold) ? 1 : 0;
            }
        }
    }
}



void Chunk::renderChunk(SDL_Renderer* renderer, const Camera camera, SDL_Texture* texture) const {
    for (size_t y = 0; y < tileData.size(); ++y) {
        for (size_t x = 0; x < tileData[y].size(); ++x) {
            // Calculate the tile's position and size, adjusted for the camera's zoom
            float worldX = ChunkCoords.x + x * 32.0f; // World X position of the tile
            float worldY = ChunkCoords.y + y * 32.0f; // World Y position of the tile

            SDL_FRect tileRect = {
                (worldX - camera.cameraPos.x) * camera.zoom, // Adjust position for camera and zoom
                (worldY - camera.cameraPos.y) * camera.zoom, // Adjust position for camera and zoom
                32.0f * camera.zoom, // Adjust tile width for zoom
                32.0f * camera.zoom  // Adjust tile height for zoom
            };

            // Set the tile color based on its type
            if (tileData[y][x] == 1) {
                SDL_RenderTexture(renderer, texture, NULL, &tileRect);
                //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for solid tiles
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Black for empty tiles
            }

            // Render the tile
            //SDL_RenderFillRect(renderer, &tileRect);
        }
    }


    
}
