#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Player.hpp"
#include "Utils.hpp"
#include "enemy.hpp"
#include "World.hpp"


Player::Player(Vector2f p_pos, SDL_Texture* p_tex, int width, int height)
    : Entity(p_pos, p_tex, width, height),
    velocity(0.0f, 0.0f),           // Initialize Player-specific members here if you add them
    speed(200.0f),
    friction(500.0f),
    horizontalAcceleration(1000.0f),
    jumpForce(500.0f),
    gravity(980.0f),
    maxFallSpeed(500.0f)                              
{
    // Any additional setup specific to Player goes here
    std::cout << "Player object fully constructed!" << std::endl;
}



void Player::update(float timeStep, 
                    const std::vector<bool>& keyStates, 
                    World& world,
                    std::vector<Enemy>& enemies) {


    velocity.y += gravity * timeStep;



    float desiredVelocityX = 0.0f;
    if (keyStates[SDL_SCANCODE_D]) {
        desiredVelocityX = speed;
    }
    if (keyStates[SDL_SCANCODE_A]) {
        desiredVelocityX = -speed;
    }

   

    if (keyStates[SDL_SCANCODE_R]) {
        pos.x = 50;
        pos.y = 50;
        velocity.y = 0;
        velocity.x = 0;
        score = 0;
        std::cout << "Player position reset to: x=" << pos.x << ", y=" << pos.y << std::endl;

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
    if (keyStates[SDL_SCANCODE_SPACE] && m_isOnGround) {
        velocity.y = -jumpForce; // Apply upward jump force (negative because Y increases downwards)
        m_isOnGround = false;
    }



    pos.y += velocity.y * timeStep;
    pos.x += velocity.x * timeStep;



    SDL_FRect playerAABB = { pos.x, pos.y, (float)getCurrentFrame().w, (float)getCurrentFrame().h };
    m_isOnGround = false; // Assume the player is not on the ground

    CollisionSide sides = world.CollidingWithTerrain(playerAABB);

    if (sides & BOTTOM) {
        // Snap to ground
        pos.y = std::floor((pos.y + playerAABB.h) / 32.0f) * 32.0f - playerAABB.h;
        velocity.y = 0;
        m_isOnGround = true;
    }
    else if (sides & TOP) {
        // Snap to ceiling
        pos.y = std::ceil(pos.y / 32.0f) * 32.0f;
        velocity.y = 0;
    }
    if (sides & LEFT) {
        pos.x = std::floor((pos.x + playerAABB.w) / 32.0f) * 32.0f - playerAABB.w;
        velocity.x = 0;
    }
    else if (sides & RIGHT) {
        pos.x = std::ceil(pos.x / 32.0f) * 32.0f;
        velocity.x = 0;
    }

    playerAABB.x = pos.x;
    playerAABB.y = pos.y;
    //float totalDeltaY = velocity.y * timeStep;
    //const float subStep = 2.0f; // or 2.0f for more precision
    //float movedY = 0.0f;
    //float signY = (totalDeltaY > 0) ? 1.0f : -1.0f;
    //m_isOnGround = false;

    //while (std::abs(movedY) < std::abs(totalDeltaY)) {
    //    float stepY = std::min(subStep, std::abs(totalDeltaY - movedY)) * signY;
    //    pos.y += stepY;
    //    SDL_FRect playerAABB = { pos.x, pos.y, (float)getCurrentFrame().w, (float)getCurrentFrame().h };
    //    CollisionSide ySide = world.CollidingWithTerrain(playerAABB);

    //    if (yside == BOTTOM) {
    //        // Snap to ground
    //        pos.y = std::floor((pos.y + playerAABB.h) / 32.0f) * 32.0f - playerAABB.h;
    //        velocity.y = 0;
    //        m_isOnGround = true;
    //        break;
    //    }
    //    else if (ySide == TOP) {
    //        // Snap to ceiling
    //        pos.y = std::ceil(pos.y / 32.0f) * 32.0f;
    //        velocity.y = 0;
    //        break;
    //    }
    //    movedY += stepY;
    //}



    //float totalDeltaX = velocity.x * timeStep;
    //const float subStepX = 2.0f;
    //float movedX = 0.0f;
    //float signX = (totalDeltaX > 0) ? 1.0f : -1.0f;

    //while (std::abs(movedX) < std::abs(totalDeltaX)) {
    //    float stepX = std::min(subStepX, std::abs(totalDeltaX - movedX)) * signX;
    //    pos.x += stepX;
    //    SDL_FRect playerAABB = { pos.x, pos.y, (float)getCurrentFrame().w, (float)getCurrentFrame().h };
    //    CollisionSide xSide = world.CollidingWithTerrain(playerAABB);

    //    if (xSide == LEFT) {
    //        pos.x = std::floor((pos.x + playerAABB.w) / 32.0f) * 32.0f - playerAABB.w;
    //        velocity.x = 0;
    //        break;
    //    }
    //    else if (xSide == RIGHT) {
    //        pos.x = std::ceil(pos.x / 32.0f) * 32.0f;
    //        velocity.x = 0;
    //        break;
    //    }
    //    movedX += stepX;
    //}

 

	// Check for collisions with enemies
    for (Enemy& e : enemies) {
        // entity bounding box
        SDL_FRect enemyAABB = { e.getPos().x, e.getPos().y, (float)e.getCurrentFrame().w, (float)e.getCurrentFrame().h };

        CollisionSide side = checkCollision(playerAABB, enemyAABB);

        if (side == BOTTOM) {
            // Move player back to the top edge of the entity
            pos.y = enemyAABB.y - playerAABB.h;
            velocity.y = 0;
            m_isOnGround = true;
        }
        if (side == TOP) {
            // Move player back to the bottom edge of the entity
            pos.y = enemyAABB.y + enemyAABB.h;
            velocity.y = 0;
        }
		if (side == LEFT) {
            // Move player back to the left edge of the entity
            pos.x = enemyAABB.x - playerAABB.w;
			velocity.x = 0;
		}
        if (side == RIGHT) {
            // Move player back to the right edge of the entity
            pos.x = enemyAABB.x + enemyAABB.w;
            velocity.x = 0;
        }

       
        playerAABB.y = pos.y; // Update AABB.y after resolving the collision
        playerAABB.x = pos.x; // Update AABB.x after resolving the collision
        

    }

    score += 1;
  





 
}
    
   





void Player::changeX(float amount) {
	pos.x += amount;

}



void Player::changeY(float amount) {
	pos.y += amount;
}

