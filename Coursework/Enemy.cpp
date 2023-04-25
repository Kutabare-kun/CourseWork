#include "Enemy.h"

#include <raymath.h>
#include <stdexcept>

#include "Pathfinding.h"
#include "function.h"
#include "Wall.h"


extern int size_box;


void Enemy::Init(float x, float y, float width, float height, Grid* grid, float speed, size_t distance)
{
    enemyRect = { x, y, width, height };
    this->grid = grid;
    this->speed = speed;
    initialPosition = { x + enemyRect.width / 2, y + enemyRect.height / 2 };
    pathDistance = distance;
}


void Enemy::Init(Rectangle rec, Grid* grid, float speed, size_t distance)
{
    enemyRect = rec;
    this->grid = grid;
    this->speed = speed;
    initialPosition = { rec.x + enemyRect.width / 2, rec.y + enemyRect.height / 2 };
    pathDistance = distance;
}


void Enemy::SetTarget(const Rectangle& target)
{
    targetRect = target;
    targetPosition = {target.x + target.width / 2, target.y + target.height / 2};
    UpdatePath();
    if (path.size() > pathDistance)
    {
        targetRect = { initialPosition.x - enemyRect.width / 2, initialPosition.y - enemyRect.height / 2, enemyRect.width, enemyRect.height };
        targetPosition = initialPosition;
        UpdatePath();
    }
}


void Enemy::Update(float delta)
{
    if (path.size() <= pathDistance         
        || (targetPosition.x == initialPosition.x && targetPosition.y == initialPosition.y))
    {
        if (ReachedTarget())
            return;

        if (path.size() > 1)
        {
            if (path.empty())
            {
                UpdatePath();
                if (path.empty())
                    return;
            }

            Vector2 direction = Vector2Subtract(path.front(), Vector2{ enemyRect.x, enemyRect.y });
            float distance = Vector2Length(direction);

            if (distance < speed * delta)
            {
                enemyRect.x = path.front().x;
                enemyRect.y = path.front().y;
                path.erase(path.begin());
            }
            else
            {
                Vector2 normalizedDirection = Vector2Normalize(direction);
                enemyRect.x += normalizedDirection.x * speed * delta;
                enemyRect.y += normalizedDirection.y * speed * delta;
            }
        }
        else
        {
            Vector2 direction = Vector2Subtract(targetPosition, Vector2{ enemyRect.x + enemyRect.width / 2.0f, enemyRect.y + enemyRect.height / 2.0f });
            Vector2 normalizedDirection = Vector2Normalize(direction);
            enemyRect.x += normalizedDirection.x * speed * delta;
            enemyRect.y += normalizedDirection.y * speed * delta;
        }
    }
}


void Enemy::Draw() const
{
#ifdef DebugFindPathEnemy
    for (const auto& vector2 : path)
        DrawRectangleV(vector2, { static_cast<float>(size_box), static_cast<float>(size_box) }, LIME);
#endif

    DrawRectangleRec(enemyRect, RED);
}


bool Enemy::ReachedTarget() const
{
	return CheckCollisionRecs(enemyRect, targetRect);
}


void Enemy::UpdatePath()
{
    Vector2 start = grid->WorldToGrid(enemyRect.x + enemyRect.width / 2.0f, enemyRect.y + enemyRect.height / 2.0f);
    Vector2 end = grid->WorldToGrid(targetPosition.x, targetPosition.y);
    path = Pathfinding::FindPath(*grid, start, end);
}

void Enemy::SetAggression(const size_t& distance)
{
    pathDistance = distance;
}


std::vector<Rectangle> Enemy::LoadData(const Image& level)
{
    if (!level.data or level.height <= 0 or level.width <= 0) throw std::runtime_error{ "Image do not load!" };

    const int CountX = level.width / size_box;
    const int CountY = level.height / size_box;

    std::vector<Rectangle> enemies;

    constexpr float enemySquare{ 10 };

    for (int x = 0; x < CountX; ++x)
        for (int y = 0; y < CountY; ++y)
            if (FindColor(level, x * size_box, y * size_box, x * size_box + size_box, y * size_box + size_box, RED))
            {
                float offsetX = (size_box - enemySquare) / 2.0f;
                float offsetY = (size_box - enemySquare) / 2.0f;

                enemies.emplace_back(static_cast<float>(x * size_box) + offsetX, static_cast<float>(y * size_box) + offsetY,
                    enemySquare, enemySquare);

                Wall::GetInstance().DeleteBox({ static_cast<float>(x * size_box), static_cast<float>(y * size_box) });
            }

    enemies.shrink_to_fit();

    return enemies;
}
