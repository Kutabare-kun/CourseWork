#include <iostream>
#include <vector>

#include "Player.h"
#include "Wall.h"


Player::Player(float x, float y, float width, float height, float speed)
    : playerRect{ x, y, width, height }, velocity{ 0, 0 }, speed{ speed } {}


void Player::Update(const float delta)
{
    std::cout << "Player X: " << playerRect.x << "\tPlayer Y: " << playerRect.y << std::endl;

    // Update player velocity based on keyboard input
    velocity.x = 0;
    velocity.y = 0;

    if (IsKeyDown(KEY_W)) {
        velocity.y -= speed;
    }
    if (IsKeyDown(KEY_S)) {
        velocity.y += speed;
    }
    if (IsKeyDown(KEY_A)) {
        velocity.x -= speed;
    }
    if (IsKeyDown(KEY_D)) {
        velocity.x += speed;
    }

    // Multiply velocity by deltaTime
	velocity.x *= delta;
    velocity.y *= delta;

    // Update player position with velocity
    playerRect.x += velocity.x;
    playerRect.y += velocity.y;

    // Perform collision detection and resolve collisions
    auto walls = Wall::get_wall();
    for (const auto& wall : walls)
        if (CheckCollisionRecs(playerRect, wall)) {
            // Resolve collision along the x-axis
            if (velocity.x > 0) {
                playerRect.x = wall.x - playerRect.width;
            }
            else if (velocity.x < 0) {
                playerRect.x = wall.x + wall.width;
            }

            // Resolve collision along the y-axis
            if (velocity.y > 0) {
                playerRect.y = wall.y - playerRect.height;
            }
            else if (velocity.y < 0) {
                playerRect.y = wall.y + wall.height;
            }
        }
}

void Player::Draw()
{
    DrawRectangleRec(playerRect, BLUE);
}
