#include "Grid.h"
#include <cmath>
#include <iostream>


void Grid::Init(int width, int height, int cellSize)
{
    this->width = width;
    this->height = height;
    this->cellSize = cellSize;
    this->walkable = std::vector<std::vector<bool>>(width, std::vector<bool>(height, true));
}


Vector2 Grid::GridToWorld(int x, int y) const
{
    return Vector2{ std::floor(static_cast<float>(x * cellSize)), std::floor(static_cast<float>(y * cellSize)) };
}


Vector2 Grid::WorldToGrid(float x, float y) const
{
    return Vector2{ std::floor(static_cast<float>(x / cellSize)), std::floor(static_cast<float>(y / cellSize)) };
}


bool Grid::IsWalkable(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return false;
    }
    return walkable[y][x];
}


void Grid::SetWalkable(int x, int y, bool isWalkable)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        walkable[y][x] = isWalkable;
}


void Grid::UpdateWalkableWithWalls(const std::vector<Rectangle>& walls)
{
    for (const Rectangle& wall : walls) 
    {
        Vector2 wallPos = WorldToGrid(wall.x, wall.y);
    	SetWalkable(wallPos.x, wallPos.y, false);
    }

    for (auto element : walkable)
    {
	    for (auto vb_reference : element)
	    {
            std::cout << vb_reference;
	    }
        std::cout << std::endl;
    }
}
