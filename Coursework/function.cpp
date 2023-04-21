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
extern std::atomic<bool> exit_console_game;
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


void ConsoleThread()
{
    std::string input;

    while (!exit_console_game)
    {
        std::unique_lock<std::mutex> ulm{ mtx };
        std::cin.clear();
        cv.wait(ulm, ConsoleOpen);

        if (exit_console_game)
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
            exit_console_game = true;

        std::this_thread::sleep_for(1s);
    }
}


bool ConsoleOpen()
{
    return predicat_cond;
}
