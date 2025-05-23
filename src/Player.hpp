#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

#include "entity.hpp"
#include "math.hpp"
#include "Utils.hpp"
#include "World.hpp"

class Enemy;


class Player : public Entity {
public:
	Player(Vector2f p_pos, SDL_Texture* p_tex, int width, int height);

	void update(float timeStep, 
				const std::vector<bool>& keyStates, 
				World& world,
				std::vector<Enemy>& enemies);

	void changeX(float amount);
	void changeY(float amount);

	bool isOnGround() const {
		return m_isOnGround; // Return the state variable
	}

	bool m_isOnGround = true; // player on ground?
	int score = 0;



private:
	float PlayerSpeed;

	Vector2f velocity;
	float speed;
	float friction;
	float horizontalAcceleration; // How quickly the player reaches max speed

	
	float jumpForce;
	float gravity;
	float maxFallSpeed;

	

};
