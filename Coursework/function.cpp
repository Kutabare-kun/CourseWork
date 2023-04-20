#include "function.h"


bool operator==(const Color& left, const Color& right)
{
    return left.a == right.a && left.r == right.r && left.g == right.g && left.b == right.b;
}


bool FindColor(const Image& level, const int& StartX, const int& StartY, const int& EndX, const int& EndY, Color color)
{
    for (int x = StartX; x < EndX; ++x)
        for (int y = StartY; y < EndY; ++y)
            if (Color color_pixel = GetImageColor(level, x, y); color_pixel == color)
                return true;

    return false;
}


std::vector<Rectangle> GetImageWall(const Image& level)
{
    const int size_box{ 64 };
    const int CountX = level.width / size_box;
    const int CountY = level.height / size_box;

    std::vector<Rectangle> wall;

    for (int x = 0; x < CountX; ++x)
        for (int y = 0; y < CountY; ++y)
            if (FindColor(level, x * size_box, y * size_box, x * size_box + size_box, y * size_box + size_box, BLACK))
                wall.emplace_back(x * size_box, y * size_box, size_box - 1, size_box - 1);

    wall.shrink_to_fit();

    return wall;
}