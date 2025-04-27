#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

#include "entity.hpp"
#include "math.hpp"


class Player;



class Enemy : public Entity {
public:
	Enemy(Vector2f p_pos, SDL_Texture* p_tex, int width, int height);

	void update(float timeStep, const std::vector<bool>& keyStates, std::vector<Entity>& entities, Player& player);

	void changeX(float amount);
	void changeY(float amount);

	bool isOnGround() const {
		return m_isOnGround; // Return the state variable
	}




private:
	float EnemySpeed;

	Vector2f velocity;
	float speed;
	float friction;
	float horizontalAcceleration; // How quickly the Enemy reaches max speed

	bool m_isOnGround = true; // Enemy on ground?
	float jumpForce;
	float gravity;

	float maxFallSpeed;


};
