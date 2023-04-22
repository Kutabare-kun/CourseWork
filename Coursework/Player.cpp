#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

#include "Player.h"
#include "Wall.h"


Player::Player(float x, float y, float width, float height, float speed)
	: playerRect{x, y, width, height}, velocity{0, 0}, speed{speed}
{
}


void Player::Update(const float delta)
{
	std::cout << "X: " << playerRect.x << "\tY: " << playerRect.y << std::endl;
	// Reset velocity
	velocity.x = 0;
	velocity.y = 0;

	// Update player velocity based on keyboard input
	if (IsKeyDown(KEY_W)) { velocity.y -= speed; }
	if (IsKeyDown(KEY_S)) { velocity.y += speed; }
	if (IsKeyDown(KEY_A)) { velocity.x -= speed; }
	if (IsKeyDown(KEY_D)) { velocity.x += speed; }

	// Normalize velocity if moving diagonally
	if (velocity.x != 0 && velocity.y != 0)
	{
		float magnitude = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
		velocity.x /= magnitude;
		velocity.y /= magnitude;
		velocity.x *= speed;
		velocity.y *= speed;
	}

	// Multiply velocity by deltaTime
	velocity.x *= delta;
	velocity.y *= delta;

	// Update player position with velocity
	playerRect.x += velocity.x;
	playerRect.y += velocity.y;

	// Border collision
	// Collision is square screenWidth * screenWidth
	if (playerRect.x < 0)
		playerRect.x = 0;
	if (playerRect.y < 0)
		playerRect.y = 0;
	if (playerRect.x + playerRect.width > GetScreenWidth())
		playerRect.x = GetScreenWidth() - playerRect.width;
	if (playerRect.y + playerRect.height > GetScreenWidth())
		playerRect.y = GetScreenWidth() - playerRect.height;


	// Perform collision detection and resolve collisions
	auto walls = Wall::GetInstance().GetWall();
	for (const auto& wall : walls)
		if (CheckCollisionRecs(playerRect, wall))
			ResolveCollision(wall); // Separate function for collision resolution
}


void Player::ResolveCollision(const Rectangle& wall)
{
	// Calculate player's AABB edges
	float leftA = playerRect.x;
	float rightA = playerRect.x + playerRect.width;
	float topA = playerRect.y;
	float bottomA = playerRect.y + playerRect.height;

	// Calculate wall's AABB edges
	float leftB = wall.x;
	float rightB = wall.x + wall.width;
	float topB = wall.y;
	float bottomB = wall.y + wall.height;

	// Calculate penetration distance for each axis
	std::array<float, 4> penetrations{rightA - leftB, rightB - leftA, bottomA - topB, bottomB - topA};

	// Find the axis with the smallest penetration
	auto minPenetrationIt = std::min_element(penetrations.begin(), penetrations.end());
	float minPenetration = *minPenetrationIt;
	size_t minIndex = std::distance(penetrations.begin(), minPenetrationIt);

	// Resolve collision based on the axis with the smallest penetration
	switch (minIndex)
	{
	case 0: 
		// Left penetration
		playerRect.x -= minPenetration;
		break;
	case 1: 
		// Right penetration
		playerRect.x += minPenetration;
		break;
	case 2: // Top penetration
		playerRect.y -= minPenetration;
		break;
	case 3: 
		// Bottom penetration
		playerRect.y += minPenetration;
		break;
	}
}


void Player::Draw()
{
	DrawRectangleRec(playerRect, BLUE);
}
