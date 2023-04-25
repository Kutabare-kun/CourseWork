#pragma once
#include "Grid.h"

class Pathfinding
{
private:
    static float Heuristic(const Vector2& a, const Vector2& b);

public:
    static std::vector<Vector2> FindPath(const Grid& grid, const Vector2& start, const Vector2& end);
};
