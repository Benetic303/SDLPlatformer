#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>

#include "math.hpp"
#include "noise.hpp"
#include "camera.hpp"



class Chunk {
public:
	Chunk()
		: ChunkCoords(0.0f, 0.0f), ChunkWidth(16 * 32), ChunkHeight(16 * 32) {
		tileData.resize(ChunkHeight / 32, std::vector<int>(ChunkWidth / 32, 0));
	}

	Chunk(Vector2f coords, int width = 16 * 32, int height = 16 * 32)
		: ChunkCoords(coords), ChunkWidth(width), ChunkHeight(height) {
		tileData.resize(ChunkHeight / 32, std::vector<int>(ChunkWidth / 32, 0));
	}


	void generateTerrain(PerlinNoise& noiseGenerator, float scale, float threshold);

	const std::vector<std::vector<int>>& getTileData() const { 
		return tileData; 
	}

	//Chunkrendering
	void renderChunk(SDL_Renderer* renderer, const Camera camera, SDL_Texture* texture) const;



private:

	int ChunkWidth;
	int ChunkHeight;
	Vector2f ChunkCoords;
	std::vector<std::vector<int>> tileData;




};