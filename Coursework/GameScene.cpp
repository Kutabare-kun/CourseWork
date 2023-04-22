#include <iostream>
#include <string>
#include <condition_variable>
#include <atomic>

#include "GameScene.h"
#include "Wall.h"

// Game Path
//--------------------------------------------------------------------------------------
extern std::string path_source;
//--------------------------------------------------------------------------------------

// Thread Console
//--------------------------------------------------------------------------------------
extern std::condition_variable cv;
extern std::atomic<bool> exit_console_game;
extern bool predicate_cond;
//--------------------------------------------------------------------------------------


GameScene::GameScene()
	: player(5, 5, 20, 20, 500)
{}

void GameScene::Update(float deltaTime)
{
	if (IsKeyDown(KEY_GRAVE))
	{
		exit_console_game = false;
		predicate_cond = !predicate_cond;
		cv.notify_one();
	}

	player.Update(deltaTime);
}


void GameScene::Draw()
{
	for (const auto& rectangle : Wall::GetInstance().GetWall())
		DrawRectangleRec(rectangle, BLACK);

	DrawLine(0, GetScreenWidth(), GetScreenWidth(), GetScreenWidth(), RED);

	player.Draw();
}


void GameScene::onActivate()
{
	Image image_level = LoadImage((path_source + R"(\source\level.png)").c_str());

	try
	{
		Wall::GetInstance().LoadData(image_level);
		Wall::GetInstance().MergeCloseRectangles();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	UnloadImage(image_level);
}


void GameScene::onDeactivate()
{
	Wall::GetInstance().Clear();
}
