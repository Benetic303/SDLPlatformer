#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Player.hpp"
#include "Utils.hpp"
#include "enemy.hpp"


Player::Player(Vector2f p_pos, SDL_Texture* p_tex, int width, int height)
    : Entity(p_pos, p_tex, width, height),
    velocity(0.0f, 0.0f),           // Initialize Player-specific members here if you add them
    speed(200.0f),
    friction(500.0f),
    horizontalAcceleration(1000.0f),
    jumpForce(400.0f),
    gravity(800.0f),
    maxFallSpeed(500.0f)
                                    
{
    // Any additional setup specific to Player goes here
    std::cout << "Player object fully constructed!" << std::endl;
}



void Player::update(float timeStep, 
                    const std::vector<bool>& keyStates, 
                    std::vector<Entity>& entities, 
                    std::vector<Enemy>& enemies) {


    velocity.y += gravity * timeStep;



    float desiredVelocityX = 0.0f;
    if (keyStates[SDLK_D]) {
        desiredVelocityX = speed;
    }
    if (keyStates[SDLK_A]) {
        desiredVelocityX = -speed;
    }

   

    if (keyStates[SDLK_R]) {
        pos.x = 50;
        pos.y = 50;
        velocity.y = 0;
        velocity.x = 0;
    }


    float velocityDiffX = desiredVelocityX - velocity.x;

    if (velocityDiffX > 0) { // Need to accelerate right or decelerate left
        velocity.x += horizontalAcceleration * timeStep;
        // Apply friction if trying to move opposite to current velocity
        if (velocity.x < 0) 
            velocity.x += friction * timeStep;
    }
    else if (velocityDiffX < 0) { // Need to accelerate left or decelerate right
        velocity.x -= horizontalAcceleration * timeStep;
        // Apply friction if trying to move opposite to current velocity
        if (velocity.x > 0) 
            velocity.x -= friction * timeStep;
    }
    else { // Desired velocity is 0 (not pressing left/right)
        // Apply friction to slow down
        if (velocity.x > 0) {
            velocity.x -= friction * timeStep;
            if (velocity.x < 0) 
                velocity.x = 0; // Prevent overshooting zero
        }
        else if (velocity.x < 0) {
            velocity.x += friction * timeStep;
            if (velocity.x > 0) 
                velocity.x = 0; // Prevent overshooting zero
        }
    }


    // Clamp horizontal velocity to max speed
    if (velocity.x > speed) 
        velocity.x = speed;

    if (velocity.x < -speed)
        velocity.x = -speed;

    // Clamp vertical velocity
    if (velocity.y > maxFallSpeed) {
        velocity.y = maxFallSpeed;
    }



    // Check for spacebar press and if the player is on the ground
    if (keyStates[SDLK_SPACE] && isOnGround()) {
        velocity.y = -jumpForce; // Apply upward jump force (negative because Y increases downwards)
        m_isOnGround = false;
        // isJumping = true; // Update jump state if you add one
    }

    SDL_FRect playerAABB = { pos.x, pos.y, (float)getCurrentFrame().w, (float)getCurrentFrame().h };


    pos.x += velocity.x * timeStep; // Update X position
    playerAABB.x = pos.x; // Update AABB's X position


	// check for horizontel collisions with entities
    for (Entity& e : entities) {
        SDL_FRect entityAABB = { e.getPos().x, e.getPos().y, (float)e.getCurrentFrame().w, (float)e.getCurrentFrame().h };

        if (checkCollision(playerAABB, entityAABB)) {
            if (velocity.x > 0) { // Moving right
                pos.x = entityAABB.x - playerAABB.w; // Place enemy to the left of the entity

            }
            else if (velocity.x < 0) { // Moving left
                pos.x = entityAABB.x + entityAABB.w; // Place enemy to the right of the entity

            }
            velocity.x = 0;
            playerAABB.x = pos.x; // Update AABB.x after resolving the collision
        }
    }

    pos.x += velocity.x * timeStep;
    playerAABB.x = pos.x; // Update AABB's X position


	//check for horizontal collisions with enemies
    for (Enemy& e : enemies) {
        // entity bounding box
        SDL_FRect enemyAABB = { e.getPos().x, e.getPos().y, (float)e.getCurrentFrame().w, (float)e.getCurrentFrame().h };

        if (checkCollision(playerAABB, enemyAABB)) {
            // Collision occurred on the X axis!

            // If moving right and collide
            if (velocity.x > 0) {
                // Move player back to the left edge of the entity
                pos.x = enemyAABB.x - playerAABB.w;
            }
            // If moving left and collide
            else if (velocity.x < 0) {
                // Move player back to the right edge of the entity
                pos.x = enemyAABB.x + enemyAABB.w;
            }
            velocity.x = 0;
            playerAABB.x = pos.x; // Update AABB.x after resolving the collision
        }
    }

   
    pos.y += velocity.y * timeStep;
    playerAABB.y = pos.y; // Update AABB's Y position

    m_isOnGround = false; // Assume the player is not on the ground

	// Check for vertical collisions with entities
    for (Entity& e : entities) {
        // entity bounding box
        SDL_FRect entityAABB = { e.getPos().x, e.getPos().y, (float)e.getCurrentFrame().w, (float)e.getCurrentFrame().h };

        if (checkCollision(playerAABB, entityAABB)) {

            // If falling down and collide
            if (velocity.y > 0) {
                // Move player back to the top edge of the entity
                pos.y = entityAABB.y - playerAABB.h;
                velocity.y = 0;
                m_isOnGround = true;
            }
            // If moving up and collide
            else if (velocity.y < 0) {
                // Move player back to the bottom edge of the entity
                pos.y = entityAABB.y + entityAABB.h;
                velocity.y = 0;
            }
            playerAABB.y = pos.y; // Update AABB.y after resolving the collision
        }
    }


    pos.y += velocity.y * timeStep;
    playerAABB.y = pos.y; // Update AABB's Y position

	// Check for vertical collisions with enemies
    for (Enemy& e : enemies) {
        // entity bounding box
        SDL_FRect enemyAABB = { e.getPos().x, e.getPos().y, (float)e.getCurrentFrame().w, (float)e.getCurrentFrame().h };

        if (checkCollision(playerAABB, enemyAABB)) {

            // If falling down and collide
            if (velocity.y > 0) {
                // Move player back to the top edge of the entity
                pos.y = enemyAABB.y - playerAABB.h;
                velocity.y = 0;
                m_isOnGround = true;
            }
            // If moving up and collide
            else if (velocity.y < 0) {
                // Move player back to the bottom edge of the entity
                pos.y = enemyAABB.y + enemyAABB.h;
                velocity.y = 0;
            }
            playerAABB.y = pos.y; // Update AABB.y after resolving the collision
        }

    }

    






    std::cout << "Player Position: (" << pos.x << ", " << pos.y << ")" << std::endl;
std::cout << "Player Velocity: (" << velocity.x << ", " << velocity.y << ")" << std::endl;
std::cout << "Player On Ground: " << (m_isOnGround ? "Yes" : "No") << std::endl;    
}
    
   





void Player::changeX(float amount) {
	pos.x += amount;

}



void Player::changeY(float amount) {
	pos.y += amount;
}

