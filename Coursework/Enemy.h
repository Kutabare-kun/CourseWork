#pragma once

#include <vector>
#include <raylib.h>
#include "Grid.h"
#include "Pathfinding.h"


// Enemy Debug
//--------------------------------------------------------------------------------------
// Show FindPath
//#define DebugFindPathEnemy
//--------------------------------------------------------------------------------------


class Enemy
{
private:
    Rectangle enemyRect;
    Rectangle targetRect;
    Vector2 targetPosition;
    Vector2 initialPosition;
    std::vector<Vector2> path;
    Grid* grid;
    float speed;
    size_t pathDistance;

public:
    Enemy() = default;

    void Init(float x, float y, float width, float height, Grid* grid, float speed, size_t distance);
    void Init(Rectangle rec, Grid* grid, float speed, size_t distance);

    void SetTarget(const Rectangle& target);
    void SetAggression(const size_t& distance);

    void Update(float delta);
    void Draw() const;

    void UpdatePath();
    bool ReachedTarget() const;

    static std::vector<Rectangle> LoadData(const Image& level);
};
