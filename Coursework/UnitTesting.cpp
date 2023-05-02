#include <string>
#include <stdexcept>

#include "UnitTesting.h"

#include "Wall.h"
#include "Pathfinding.h"

extern std::string path_source;
extern int size_box;

constexpr void assert_point(bool statement, const char* message)
{
	if (!statement) throw std::runtime_error{ message };
}


void UNIT_TEST(void(*unit_test)(), const char* name)
{
	try {
		unit_test();
		printf("[+] Test %s successful.\n", name);
	}
	catch (const std::exception& e) {
		printf("[-] Test failure in %s. %s.\n", name, e.what());
	}
}


void TEST_NoObstacles()
{
	Image level = LoadImage((path_source + R"(\source\TEST_NoObstacles.png)").c_str());
	Wall::GetInstance().LoadData(level);

	Grid grid;
	grid.Init(level.width / size_box, level.height / size_box, size_box);
	grid.UpdateWalkableWithWalls(Wall::GetInstance().GetWall());
	UnloadImage(level);

	std::vector<Vector2> path = Pathfinding::FindPath(grid, grid.WorldToGrid(0, 0), grid.WorldToGrid(544, 544));

	for (const auto& rectangle : Wall::GetInstance().GetWall())
		DrawRectangleRec(rectangle, BLACK);

	for (const auto & value : path)
		DrawRectangleV(value, { static_cast<float>(size_box), static_cast<float>(size_box) }, GREEN);

	DrawRectangle(0, 0, size_box, size_box, RED);
	DrawRectangle(544, 544, size_box, size_box, ORANGE);

	Wall::GetInstance().Clear();
	assert_point(!path.empty(), "Couldn't find the path");
}


void TEST_WithObstacles()
{
	Image level = LoadImage((path_source + R"(\source\TEST_WithObstacles.png)").c_str());
	Wall::GetInstance().LoadData(level);

	Grid grid;
	grid.Init(level.width / size_box, level.height / size_box, size_box);
	grid.UpdateWalkableWithWalls(Wall::GetInstance().GetWall());
	UnloadImage(level);

	std::vector<Vector2> path = Pathfinding::FindPath(grid, grid.WorldToGrid(0, 0), grid.WorldToGrid(544, 544));

	for (const auto& rectangle : Wall::GetInstance().GetWall())
		DrawRectangleRec(rectangle, BLACK);

	for (const auto& value : path)
		DrawRectangleV(value, { static_cast<float>(size_box), static_cast<float>(size_box) }, GREEN);

	DrawRectangle(0, 0, size_box, size_box, RED);
	DrawRectangle(544, 544, size_box, size_box, ORANGE);

	Wall::GetInstance().Clear();
	assert_point(!path.empty(), "Couldn't find the path");
}


void TEST_NoPath()
{
	Image level = LoadImage((path_source + R"(\source\TEST_NoPath.png)").c_str());
	Wall::GetInstance().LoadData(level);

	Grid grid;
	grid.Init(level.width / size_box, level.height / size_box, size_box);
	grid.UpdateWalkableWithWalls(Wall::GetInstance().GetWall());
	UnloadImage(level);

	std::vector<Vector2> path = Pathfinding::FindPath(grid, grid.WorldToGrid(0, 0), grid.WorldToGrid(544, 544));

	for (const auto& rectangle : Wall::GetInstance().GetWall())
		DrawRectangleRec(rectangle, BLACK);

	for (const auto& value : path)
		DrawRectangleV(value, { static_cast<float>(size_box), static_cast<float>(size_box) }, GREEN);

	DrawRectangle(0, 0, size_box, size_box, RED);
	DrawRectangle(544, 544, size_box, size_box, ORANGE);

	Wall::GetInstance().Clear();
	assert_point(path.empty(), "Couldn't find the path");
}
