#include <stdexcept>
#include <ranges>

#include "Wall.h"
#include "function.h"


extern int size_box;


Wall::Wall()
	: objects()
{}


Wall& Wall::GetInstance()
{
	static Wall instance;
	return instance;
}


void Wall::LoadData(const Image& level)
{
	if (!level.data or level.height <= 0 or level.width <= 0) throw std::runtime_error{"Image do not load!"};

	const int CountX = level.width / size_box;
	const int CountY = level.height / size_box;

	objects.clear();

	for (int x = 0; x < CountX; ++x)
		for (int y = 0; y < CountY; ++y)
			if (FindColor(level, x * size_box, y * size_box, x * size_box + size_box, y * size_box + size_box, BLACK))
				objects.emplace_back(x * size_box, y * size_box, size_box, size_box);

	objects.shrink_to_fit();
}


void Wall::MergeCloseRectangles()
{
	constexpr float distance{1.f};

	for (size_t i = 0; i < objects.size(); ++i)
	{
		for (size_t j = 0; j < objects.size(); ++j)
		{
			if (i == j)
				continue;

			Rectangle& rect_1 = objects[i];
			Rectangle& rect_2 = objects[j];

			bool changed{false};
			if (abs(rect_1.x - rect_2.x) <= 0.01
				&& abs(rect_1.width - rect_2.width) <= 0.01)
			{
				float distance_y_top = rect_1.y - (rect_2.y + rect_2.height);
				float distance_y_bottom = (rect_1.y + rect_1.height) - rect_2.y;

				if (abs(distance_y_top) <= 0.01)
				{
					rect_1.y -= rect_2.height;
					rect_1.height += rect_2.height;
					changed = true;
				}
				else if (abs(distance_y_bottom) <= 0.01)
				{
					rect_1.height += rect_2.height;
					changed = true;
				}

				if (changed)
				{
					objects.erase(objects.begin() + j);
					--i;
					break;
				}
			}
			else if (abs(rect_1.y - rect_2.y) <= 0.01
				&& abs(rect_1.height - rect_2.height) <= 0.01)
			{
				float distance_y_left = rect_1.x - (rect_2.x + rect_2.width);
				float distance_y_right = (rect_1.x + rect_1.width) - rect_2.x;

				if (abs(distance_y_left) <= 0.01)
				{
					rect_1.x -= rect_2.width;
					rect_1.width += rect_2.width;
					changed = true;
				}
				else if (abs(distance_y_right) <= 0.01)
				{
					rect_1.width += rect_2.width;
					changed = true;
				}

				if (changed)
				{
					objects.erase(objects.begin() + j);
					--i;
					break;
				}
			}
		}
	}
}


void Wall::Clear()
{
	objects.clear();
}


void Wall::DeleteBox(const Vector2& point)
{
	for (size_t i{}; i < objects.size(); ++i)
		if (abs(objects[i].x - point.x) <= 0.01
			&& abs(objects[i].y - point.y) <= 0.01)
		{
			objects.erase(objects.begin() + i);
			break;
		}

	objects.shrink_to_fit();
}
