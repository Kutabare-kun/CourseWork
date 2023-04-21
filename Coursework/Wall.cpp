#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <ranges>

#include "Wall.h"
#include "function.h"


std::vector<Rectangle> Wall::objects;


Wall& Wall::GetInstance()
{
	static Wall instance;
	return instance;
}


void Wall::LoadData(const Image& level)
{
    if (!level.data or level.height <= 0 or level.width <= 0) throw std::runtime_error{ "Image do not load!" };

    constexpr int size_box{ 64 }, height_width{ size_box - 1 };
    const int CountX = level.width / size_box;
    const int CountY = level.height / size_box;

    objects.clear();

    for (int x = 0; x < CountX; ++x)
        for (int y = 0; y < CountY; ++y)
            if (FindColor(level, x * size_box, y * size_box, x * size_box + size_box, y * size_box + size_box, BLACK))
                objects.emplace_back(x * size_box, y * size_box, height_width, height_width);

    objects.shrink_to_fit();
}


void Wall::LoadData(const std::string& path)
{
    if (const auto extension = std::filesystem::path(path).extension(); 
        extension.compare(".csv") or extension.compare(".CSV")) 
        throw std::runtime_error{ "File is not *.csv" };

    std::fstream file{ path, std::ios_base::in };

    if (!file) throw std::runtime_error{ "Unable to open file: " + path };

    constexpr int size_box{ 64 }, height_width{ size_box - 1 };
    objects.clear();

    std::string line;
    size_t y{};
    while (std::getline(file, line))
    {
        for (const auto& value : line | std::ranges::views::split('\r'))
        {
            size_t x{};
            std::string row{ value.begin(),value.end() };
            for (const auto& column : row | std::ranges::views::split(','))
            {
                if (std::string index = { column.begin(), column.end() }; !index.empty())
                {
                    if (index.size() > 1) throw std::runtime_error{ "File has incorrect cell" };
                    else if (index == "1")
                        objects.emplace_back(x * size_box, y * size_box, height_width, height_width);
                }
                else throw std::runtime_error{ "File has empty cell" };

                ++x;
            }

            ++y;
        }
    }

    objects.shrink_to_fit();
}
