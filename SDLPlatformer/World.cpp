#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>

#include "World.hpp"
#include "math.hpp"
#include "noise.hpp"
#include "Chunk.hpp"
#include "Utils.hpp"



void World::loadChunk(Vector2f coords, PerlinNoise& noiseGenerator, float scale, float threshold) {
    if (chunks.find(coords) == chunks.end()) {
        Chunk newChunk(coords);
        newChunk.generateTerrain(noiseGenerator, scale, threshold);
        chunks[coords] = newChunk;
    }
}

const Chunk* World::getChunk(Vector2f coords) const {
    auto it = chunks.find(coords);
    return it != chunks.end() ? &it->second : nullptr;
}

//CollisionSide World::CollidingWithTerrain(const SDL_FRect& rect) const {
//    const int tileSize = 32; // Tile size in pixels
//    const int chunkSizeInPixels = 16 * tileSize; // Chunk size in pixels
//
//    // Calculate the range of chunks the player's bounding box overlaps
//    Vector2f topLeftChunkCoords(
//        std::floor(rect.x / chunkSizeInPixels) * chunkSizeInPixels,
//        std::floor(rect.y / chunkSizeInPixels) * chunkSizeInPixels
//    );
//    Vector2f bottomRightChunkCoords(
//        std::floor((rect.x + rect.w) / chunkSizeInPixels) * chunkSizeInPixels,
//        std::floor((rect.y + rect.h) / chunkSizeInPixels) * chunkSizeInPixels
//    );
//
//    // Iterate over all overlapping chunks
//    for (int chunkY = topLeftChunkCoords.y; chunkY <= bottomRightChunkCoords.y; chunkY += chunkSizeInPixels) {
//        for (int chunkX = topLeftChunkCoords.x; chunkX <= bottomRightChunkCoords.x; chunkX += chunkSizeInPixels) {
//            Vector2f chunkCoords(chunkX, chunkY);
//
//            // Check if the chunk exists
//            auto it = chunks.find(chunkCoords);
//            if (it != chunks.end()) {
//                const Chunk& chunk = it->second;
//                const auto& tileData = chunk.getTileData();
//
//                // Iterate over the tiles in the chunk
//                for (size_t tileY = 0; tileY < tileData.size(); ++tileY) {
//                    for (size_t tileX = 0; tileX < tileData[tileY].size(); ++tileX) {
//                        if (tileData[tileY][tileX] == 1) { // Solid tile
//                            SDL_FRect tileRect = {
//                                chunkCoords.x + tileX * tileSize,
//                                chunkCoords.y + tileY * tileSize,
//                                (float)tileSize,
//                                (float)tileSize
//                            };
//
//                            // Check for collision with the player's bounding box
//                            if (SDL_HasRectIntersectionFloat(&rect, &tileRect)) {
//                                // Determine the collision side
//                                return checkCollision(rect, tileRect);
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    return NONE; // No collision
//}

CollisionSide World::CollidingWithTerrain(const SDL_FRect& rect) const {
    const int tileSize = 32; // Tile size in pixels
    const int chunkSizeInPixels = 16 * tileSize; // Chunk size in pixels

    // Calculate the chunk coordinates for the rect
    Vector2f chunkCoords(
        std::floor(rect.x / chunkSizeInPixels) * chunkSizeInPixels,
        std::floor(rect.y / chunkSizeInPixels) * chunkSizeInPixels
    );

    std::cout << "Chunk Coords: x=" << chunkCoords.x
        << ", y=" << chunkCoords.y << std::endl; //Debugging

    auto it = chunks.find(chunkCoords);
    if (it != chunks.end()) {
        const Chunk& chunk = it->second; // Access the chunk
        // Now you can check for collisions with blocks in this chunk
    }
    if (it != chunks.end()) {
        const Chunk& chunk = it->second;
        const auto& tileData = chunk.getTileData();

        

        // Iterate over the tiles in the chunk
        for (size_t tileY = 0; tileY < tileData.size(); ++tileY) {
            for (size_t tileX = 0; tileX < tileData[tileY].size(); ++tileX) {
                if (tileData[tileY][tileX] == 1) { // Solid tile
                    SDL_FRect tileRect = {
                        chunkCoords.x + tileX * tileSize,
                        chunkCoords.y + tileY * tileSize,
                        (float)tileSize,
                        (float)tileSize
                    };
                   

                    // Check for collision with the rect
                    if (SDL_HasRectIntersectionFloat(&rect, &tileRect)) {
                        // Collision detected
                        std::cout << "Collision with tile at (" << tileX << ", " << tileY << ")" << std::endl;
                        return checkCollision(rect, tileRect); // Return the collision side
                    }
                }
            }
        }
    }
    
    
    
    
    return NONE; // No collision

}