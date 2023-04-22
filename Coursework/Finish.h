#pragma once
#include <raylib.h>

#include "Player.h"

class Finish
{
private:
	Rectangle area;

public:
	Finish() = default;

	void SetFinishArea(float x, float y, float width, float height);
	bool PlayerWon(Player& player);
	void LoadData(const Image& level);
	void LoadData(const std::string& path);
	void Draw();
};

