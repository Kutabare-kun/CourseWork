#pragma once
#include <raylib.h>
#include <string>
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
	void LoadData(const std::string& path);
	void MergeCloseRectangles();
	void Clear();
	const std::vector<Rectangle>& GetWall() { return objects; }

};
