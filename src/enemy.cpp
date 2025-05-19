#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "enemy.hpp"
#include "Utils.hpp"
#include "Player.hpp"
#include "World.hpp"


Enemy::Enemy(Vector2f p_pos, SDL_Texture* p_tex, int width, int height)
    : Entity(p_pos, p_tex, width, height),
    velocity(0.0f, 0.0f),           // Initialize Player-specific members here if you add them
    speed(150.0f),
    friction(500.0f),
    horizontalAcceleration(700.0f),
    jumpForce(400.0f),
    gravity(980.0f),
    EnemySpeed(0.0f), // Initialize EnemySpeed to avoid uninitialized variable warning
    maxFallSpeed(500.0f)
{
    // Any additional setup specific to Player goes here
    std::cout << "Player object fully constructed!" << std::endl;
}



void Enemy::update(float timeStep, const std::vector<bool>& keyStates, World& world, Player& player) {


    velocity.y += gravity * timeStep;


    float desiredVelocityX = 0.0f;
    

    // Example AI logic: Move toward the player
    if (player.getPos().x > pos.x) {
        desiredVelocityX = speed;
    }
    else if (player.getPos().x < pos.x) {
        desiredVelocityX = -speed;
    }
    if (player.getPos().y < pos.y && isOnGround()) {
        velocity.y = -jumpForce; // Apply upward jump force (negative because Y increases downwards)
        m_isOnGround = false;
        // isJumping = true; // Update jump state if you add one
    }
	
    

    if (keyStates[SDL_SCANCODE_R]) {
        pos.x = 100;
        pos.y = 200;
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

    if (velocity.y > maxFallSpeed) { // Define maxFallSpeed if necessary
        velocity.y = maxFallSpeed;
    }
 

    m_isOnGround = false; // Assume the player is not on the ground
    
    SDL_FRect enemyAABB = { pos.x, pos.y, (float)getCurrentFrame().w, (float)getCurrentFrame().h };

    pos.x += velocity.x * timeStep; // Update X position
    pos.y += velocity.y * timeStep; // Update Y position
    enemyAABB.y = pos.y; // Update AABB's Y position
	enemyAABB.x = pos.x; // Update AABB's X position
    m_isOnGround = false; // Assume the enemy is not on the ground

    /*CollisionSide side = world.CollidingWithTerrain(currentFrame);

    if (side != NONE) {
        if (side == BOTTOM) {
            velocity.y = 0;
            m_isOnGround = true;
        }
        else if (side == TOP) {
            velocity.y = 0;
        }
        else if (side == LEFT || side == RIGHT) {
            velocity.x = 0;
        }
    }
    else {
        m_isOnGround = false;
    }*/

    enemyAABB.y = pos.y; // Update AABB.y after resolving the collision
    enemyAABB.x = pos.x; // Update AABB.x after resolving the collision


    // --- Vertical Movement and Collision ---


    // --- Handle Collision with the Player ---
    SDL_FRect playerAABB = { player.getPos().x, player.getPos().y, (float)player.getCurrentFrame().w, (float)player.getCurrentFrame().h };

    if (checkCollision(enemyAABB, playerAABB) != NONE) {

        CollisionSide side = checkCollision(enemyAABB, playerAABB);

        if (player.getPos().y + player.getCurrentFrame().h <= pos.y + 5) { // Player is on top of the enemy
            player.changeX(-velocity.x * timeStep); // Move the player with the enemy
            player.m_isOnGround = true; // Ensure the player is considered on the ground
        }
        else {
            if (side == BOTTOM) {
                // Move player back to the top edge of the entity
                pos.y = playerAABB.y - enemyAABB.h;
                velocity.y = 0;
                m_isOnGround = true;
            }
            if (side == TOP) {
                // Move player back to the bottom edge of the entity
                pos.y = playerAABB.y + playerAABB.h;
                velocity.y = 0;
            }
            if (side == LEFT) {
                pos.x = playerAABB.x - enemyAABB.w; // Place enemy to the left of the entity
                velocity.x = 0;
            }
            if (side == RIGHT) {
                pos.x = playerAABB.x + playerAABB.w; // Place enemy to the right of the entity
                velocity.x = 0;
            }
            enemyAABB.y = pos.y; // Update AABB.y after resolving the collision
            enemyAABB.x = pos.x; // Update AABB.x after resolving the collision
        }
    }


   
}







void Enemy::changeX(float amount) {
    pos.x += amount;

}



void Enemy::changeY(float amount) {
    pos.y += amount;
}

