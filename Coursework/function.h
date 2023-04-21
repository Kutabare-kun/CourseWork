#pragma once
#include <raylib.h>

bool operator==(const Color& left, const Color& right);
bool operator==(const Rectangle& left, const Rectangle& right);

bool FindColor(const Image& level, const int& StartX, const int& StartY, const int& EndX, const int& EndY, Color color);
void ConsoleThread();
bool ConsoleOpen();