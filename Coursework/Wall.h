#pragma once
#include <raylib.h>
#include <vector>

class Wall
{
private:
	Wall();

	std::vector<Rectangle> objects;

public:
	Wall(const Wall&) = delete;
	Wall& operator=(const Wall&) = delete;

	static Wall& GetInstance();
	void LoadData(const Image& level);
	void MergeCloseRectangles();
	void Clear();
	void DeleteBox(const Vector2& point);
	const std::vector<Rectangle>& GetWall() { return objects; }

};
