#pragma once
#include <raylib.h>
#include <vector>

#include "Grid.h"
#include "Pathfinding.h"

class Enemy
{
private:
    Vector2 position;
    Vector2 initialPosition;
    float speed;
    int radius;

    std::vector<Vector2> path;

public:
    Enemy(float x, float y, float speed, int radius);

    void Update(float delta, const Vector2& playerPosition, const Grid& grid);
    void Draw();

    const Vector2& GetPosition() const { return position; }
};
