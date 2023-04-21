#pragma once
#include <raylib.h>
#include <vector>

class Wall
{
private:
    Wall() = default;

    static std::vector<Rectangle> objects;

public:
    Wall(const Wall&) = delete;
    Wall& operator=(const Wall&) = delete;

    static Wall& GetInstance();
    static void LoadData(const Image& level);
    static void LoadData(const std::string& path);
    static const std::vector<Rectangle>& get_wall() { return objects; }
};
