#pragma once
#include <raylib.h>

bool operator==(const Color& left, const Color& right);

bool FindColor(const Image& level, const int& StartX, const int& StartY, const int& EndX, const int& EndY, Color color);
bool OutOfScreen(const Vector2& pos);
void ConsoleThread();
bool ConsoleOpen();
