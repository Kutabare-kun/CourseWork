#pragma once
#include <raylib.h>
#include <string>
#include <vector>

bool operator==(const Color& left, const Color& right);
bool FindColor(const Image& level, const int& StartX, const int& StartY, const int& EndX, const int& EndY, Color color);
std::vector<Rectangle> GetWall(const Image& level);
std::vector<Rectangle> GetWall(const std::string& path);
void ConsoleThread();
bool ConsoleOpen();