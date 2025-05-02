#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>
#include <map>

#include "Chunk.hpp"
#include "math.hpp"
#include "noise.hpp"
#include "Utils.hpp"




class World {
public:
	void loadChunk(Vector2f coords, PerlinNoise& noiseGenerator, float scale, float threshold);

    const Chunk* getChunk(Vector2f coords) const;

    CollisionSide CollidingWithTerrain(const SDL_FRect& rect) const;

    const std::map<Vector2f, Chunk>& getChunks() const { 
        return chunks; 
    }
private:
    std::map<Vector2f, Chunk> chunks; // Map of chunks by their coordinate
};