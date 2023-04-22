#pragma once

#include <vector>
#include <raylib.h>
#include "Grid.h"
#include "Pathfinding.h"

class Enemy
{
private:
    Rectangle enemyRect;
    Vector2 targetPosition;
    Vector2 initialPosition;
    std::vector<Vector2> path;
    Grid* grid;
    float speed;

public:
    Enemy() = default;

    void Init(float x, float y, float width, float height, Grid* grid, float speed);
    void Init(Rectangle rec, Grid* grid, float speed);

    void SetTarget(const Vector2& target);
    void Update(float delta);
    void Draw() const;
    bool ReachedTarget() const;
    void UpdatePath();

    static std::vector<Rectangle> LoadData(const Image& level);
};
