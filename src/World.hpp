#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>


#include "Chunk.hpp"
#include "math.hpp"
#include "noise.hpp"
#include "Utils.hpp"
#include "CollisionSide.hpp"



class World {
public:
	void loadChunk(Vector2f coords, PerlinNoise& noiseGenerator, float scale, float threshold);

    void startChunkGenerationThread();
    void stopChunkGenerationThread();
    void enqueueChunk(Vector2f coords, PerlinNoise& noiseGenerator, float scale, float threshold);


    const Chunk* getChunk(Vector2f coords) const;

    CollisionSide CollidingWithTerrain(const SDL_FRect& rect) const;

    const std::map<Vector2f, Chunk>& getChunks() const { 
        return chunks; 
    }

    void printLoadedChunks() const {
        std::cout << "Loaded chunks:" << std::endl;
        for (const auto& [coords, chunk] : chunks) {
            std::cout << "Chunk at x=" << coords.x << ", y=" << coords.y << std::endl;
        }
    }

private:
    std::map<Vector2f, Chunk> chunks; // Map of chunks by their coordinate

    std::queue<std::tuple<Vector2f, PerlinNoise, float, float>> chunkQueue;
    std::mutex chunkMutex;
    std::condition_variable chunkCondition;
    bool stopThread = false;
    std::thread chunkThread;

    void chunkGenerationWorker();

};