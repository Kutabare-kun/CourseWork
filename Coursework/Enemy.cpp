#include "Enemy.h"


void Enemy::Update(float delta, const Player& player)
{
    FollowPlayer(player, 32.0f);

    // Update enemy position with velocity
    enemyRect.x += velocity.x * delta;
    enemyRect.y += velocity.y * delta;
}


void Enemy::Draw()
{
    DrawRectangleRec(enemyRect, RED);
}


void Enemy::FollowPlayer(const Player& player, float radius)
{
    // Reset velocity
    velocity.x = 0;
    velocity.y = 0;

    // Calculate distance between the enemy and the player
    Vector2 playerCenter = { player.GetPlayerRect().x + player.GetPlayerRect().width / 2,
                            player.GetPlayerRect().y + player.GetPlayerRect().height / 2 };
    Vector2 enemyCenter = { enemyRect.x + enemyRect.width / 2, enemyRect.y + enemyRect.height / 2 };

    Vector2 diff = { playerCenter.x - enemyCenter.x, playerCenter.y - enemyCenter.y };
    float distance = sqrt(diff.x * diff.x + diff.y * diff.y);

    // Follow the player if the distance is less than the given radius
    if (distance < radius)
    {
        float normalizedX = diff.x / distance;
        float normalizedY = diff.y / distance;

        velocity.x = normalizedX * speed;
        velocity.y = normalizedY * speed;
    }
}
