#include <filesystem>
#include <ranges>
#include <stdexcept>
#include <fstream>

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


void Finish::LoadData(const std::string& path)
{
	if (const auto extension = std::filesystem::path(path).extension();
		extension.compare(".csv") or extension.compare(".CSV"))
		throw std::runtime_error{ "File is not *.csv" };

	std::fstream file{ path, std::ios_base::in };

	if (!file) throw std::runtime_error{ "Unable to open file: " + path };

	std::string line;
	size_t y{};
	while (std::getline(file, line))
	{
		for (const auto& value : line | std::ranges::views::split('\r'))
		{
			size_t x{};
			std::string row{ value.begin(), value.end() };
			for (const auto& column : row | std::ranges::views::split(','))
			{
				if (std::string index = { column.begin(), column.end() }; !index.empty())
				{
					if (index.size() > 1) throw std::runtime_error{ "File has incorrect cell" };
					if (index == "9")
					{
						area = { static_cast<float>(x * size_box), static_cast<float>(y * size_box),
						static_cast<float>(size_box), static_cast<float>(size_box) };
						return;
					}
				}
				else throw std::runtime_error{ "File has empty cell" };

				++x;
			}

			++y;
		}
	}
}


void Finish::Draw()
{
	DrawRectangleRec(area, ORANGE);
}
