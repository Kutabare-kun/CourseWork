#pragma once
#include <raylib.h>
#include <vector>

class Grid
{
public:
    Grid() = default;

    void Init(int width, int height, int cellSize);

    Vector2 GridToWorld(int x, int y) const;
    Vector2 WorldToGrid(float x, float y) const;

    void Clear();

    bool IsWalkable(int x, int y) const;
    void SetWalkable(int x, int y, bool walkable);

    const std::vector<std::vector<bool>>& GetGrid() const { return walkable; }

    void UpdateWalkableWithWalls(const std::vector<Rectangle>& walls);

private:
    int width, height;
    int cellSize;
    std::vector<std::vector<bool>> walkable;
};
