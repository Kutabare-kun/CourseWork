#include <ranges>
#include <stdexcept>

#include "Finish.h"
#include "function.h"
#include "Wall.h"


extern int size_box;


void Finish::SetFinishArea(float x, float y, float width, float height)
{
	area = { x, y ,width ,height };
}


bool Finish::PlayerWon(Player& player)
{
	return CheckCollisionRecs(player.GetPlayerRect(), area);
}


void Finish::LoadData(const Image& level)
{
	if (!level.data or level.height <= 0 or level.width <= 0) throw std::runtime_error{ "Image do not load!" };

	const int CountX = level.width / size_box;
	const int CountY = level.height / size_box;

	for (int x = 0; x < CountX; ++x)
		for (int y = 0; y < CountY; ++y)
			if (FindColor(level, x * size_box, y * size_box, x * size_box + size_box, y * size_box + size_box, ORANGE))
			{
				area = { static_cast<float>(x * size_box), static_cast<float>(y * size_box),
				static_cast<float>(size_box), static_cast<float>(size_box) };
				Wall::GetInstance().DeleteBox({ static_cast<float>(x * size_box), static_cast<float>(y * size_box) });
				return;
			}
}


void Finish::Draw()
{
	DrawRectangleRec(area, ORANGE);
}
