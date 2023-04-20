#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <atomic>
#include <chrono>

#include "function.h"


using namespace std::chrono_literals;


extern std::mutex mtx;
extern std::condition_variable cv;
extern std::atomic<bool> exitGame;
extern bool predicat_cond;


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


std::vector<Rectangle> GetWall(const Image& level)
{
    if (!level.data or level.height <= 0 or level.width <= 0) throw std::runtime_error{ "Image do not load!" };

    constexpr int size_box{ 64 }, height_width{size_box - 1};
    const int CountX = level.width / size_box;
    const int CountY = level.height / size_box;

    std::vector<Rectangle> wall;

    for (int x = 0; x < CountX; ++x)
        for (int y = 0; y < CountY; ++y)
            if (FindColor(level, x * size_box, y * size_box, x * size_box + size_box, y * size_box + size_box, BLACK))
                wall.emplace_back(x * size_box, y * size_box, height_width, height_width);

    wall.shrink_to_fit();

    return wall;
}


std::vector<Rectangle> GetWall(const std::string& path)
{
    if (std::filesystem::path(path).extension() != ".csv") throw std::runtime_error{ "File is not *.csv" };

    std::fstream file{ path, std::ios_base::in };

    if (!file) throw std::runtime_error{ "Unable to open file: " + path };

    constexpr int size_box{ 64 }, height_width{ size_box - 1 };
    std::vector<Rectangle> wall;

    std::string line;
    size_t y{};
    while(std::getline(file, line))
    {
	    for (const auto& value : line | std::ranges::views::split('\r'))
	    {
            size_t x{};
            std::string row{ value.begin(),value.end() };
            for (auto column : row | std::ranges::views::split(','))
            {
	            if (std::string index = { column.begin(), column.end() }; !index.empty())
	            {
	                if (index.size() > 1) throw std::runtime_error{ "File has incorrect cell" };
	                else if (index == "1")
	                    wall.emplace_back(x * size_box, y * size_box, height_width, height_width);
	            }
	            else throw std::runtime_error{ "File has empty cell" };

                ++x;
            }

            ++y;
	    }
    }

    wall.shrink_to_fit();

    return wall;
}


void ConsoleThread()
{
    std::string input;

    while (!exitGame)
    {
        std::unique_lock<std::mutex> ulm{ mtx };
        std::cin.clear();
        cv.wait(ulm, ConsoleOpen);

        if (exitGame)
	        break;

        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        if (!input.empty())
        {
            // Process input
            std::cout << "Received input: " << input << std::endl;

            predicat_cond = false;
        }

        if (input == "exit")
        {
            exitGame = true;
        }

        std::this_thread::sleep_for(1s);
    }
}


bool ConsoleOpen()
{
    return predicat_cond;
}
