#pragma once
#include <raylib.h>
#include <string>
#include <vector>

bool operator==(const Color& left, const Color& right);
bool FindColor(const Image& level, const int& StartX, const int& StartY, const int& EndX, const int& EndY, Color color);
void ConsoleThread();
bool ConsoleOpen();