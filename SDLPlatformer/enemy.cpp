#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "enemy.hpp"
#include "Utils.hpp"
#include "Player.hpp"



Enemy::Enemy(Vector2f p_pos, SDL_Texture* p_tex, int width, int height)
    : Entity(p_pos, p_tex, width, height),
    velocity(0.0f, 0.0f),           // Initialize Player-specific members here if you add them
    speed(50.0f),
    friction(500.0f),
    horizontalAcceleration(100.0f),
    jumpForce(200.0f),
    gravity(800.0f),
    EnemySpeed(0.0f) // Initialize EnemySpeed to avoid uninitialized variable warning


{
    // Any additional setup specific to Player goes here
    std::cout << "Player object fully constructed!" << std::endl;
}



void Enemy::update(float timeStep, const std::vector<bool>& keyStates, std::vector<Entity>& entities, Player& player) {


    velocity.y += gravity * timeStep;


    float desiredVelocityX = 0.0f;
    

    // Example AI logic: Move toward the player
    if (player.getPos().x > pos.x) {
        desiredVelocityX = speed;
    }
    else if (player.getPos().x < pos.x) {
        desiredVelocityX = -speed;
    }

    if (keyStates[SDLK_R]) {
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


 
     

    SDL_FRect enemyAABB = { pos.x, pos.y, (float)getCurrentFrame().w, (float)getCurrentFrame().h };

    pos.x += velocity.x * timeStep; // Update X position
    enemyAABB.x = pos.x; // Update AABB's X position

    // Check for collisions with the player
    SDL_FRect playerAABB = { player.getPos().x, player.getPos().y,
                             (float)player.getCurrentFrame().w, (float)player.getCurrentFrame().h };

    if (checkCollision(enemyAABB, playerAABB)) {
        // Handle collision with the player
        std::cout << "Enemy collided with the player!" << std::endl;

        // Example: Stop the enemy's movement upon collision
        velocity.x = 0;
        velocity.y = 0;

        // Example: Push the player back
        if (pos.x < player.getPos().x) {
            //player.changeX(10.0f); // Push player to the right
        }
        else {
            //player.changeX(-10.0f); // Push player to the left
        }
    }

    pos.x += velocity.x * timeStep; // Update X position
    enemyAABB.x = pos.x; // Update AABB's X position
    // check for horizontel collisions
    for (Entity& e : entities) {
        // entity bounding box
        SDL_FRect entityAABB = { e.getPos().x, e.getPos().y, (float)e.getCurrentFrame().w, (float)e.getCurrentFrame().h };

        if (checkCollision(enemyAABB, entityAABB)) {
            // Collision occurred on the X axis!

            // If moving right and collide
            if (velocity.x > 0) {
                // Move enemy back to the left edge of the entity
                pos.x = entityAABB.x - enemyAABB.w;
                velocity.x = 0;
            }
            // If moving left and collide
            else if (velocity.x < 0) {
                // Move player back to the right edge of the entity
                pos.x = entityAABB.x + entityAABB.w;
                velocity.x = 0;
            }
            enemyAABB.x = pos.x; // Update AABB.x after resolving the collision
        }
    }

    pos.y += velocity.y * timeStep; // Update X position
    enemyAABB.y = pos.y; // Update AABB's X position

    // Check for vertical collisions
    for (Entity& e : entities) {
        // entity bounding box
        SDL_FRect entityAABB = { e.getPos().x, e.getPos().y, (float)e.getCurrentFrame().w, (float)e.getCurrentFrame().h };

        if (checkCollision(enemyAABB, entityAABB)) {

            // If falling down and collide
            if (velocity.y > 0) {
                // Move player back to the top edge of the entity
                pos.y = entityAABB.y - enemyAABB.h;
                velocity.y = 0;
                m_isOnGround = true;
            }
            // If moving up and collide
            else if (velocity.y < 0) {
                // Move player back to the bottom edge of the entity
                pos.y = entityAABB.y + entityAABB.h;
                velocity.y = 0;
            }
            enemyAABB.y = pos.y; // Update AABB.y after resolving the collision
        }
    }

   
}







void Enemy::changeX(float amount) {
    pos.x += amount;

}



void Enemy::changeY(float amount) {
    pos.y += amount;
}

