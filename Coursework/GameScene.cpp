#include <iostream>
#include <string>
#include <condition_variable>
#include <atomic>

#include "GameScene.h"
#include "Wall.h"
#include "PlayerFactory.h"


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

// Temp
//--------------------------------------------------------------------------------------
size_t currect_player_temp = 0;
//--------------------------------------------------------------------------------------


GameScene::GameScene()
	: players(), finish()
{}


void GameScene::Update(float deltaTime)
{
	if (IsKeyDown(KEY_GRAVE))
	{
		exit_console_game = false;
		predicate_cond = !predicate_cond;
		cv.notify_one();
	}

	if (!finish.PlayerWon(*players[currect_player_temp]) and players[currect_player_temp]->IsAlive())
		players[currect_player_temp]->Update(deltaTime);
}


void GameScene::Draw()
{
	if (finish.PlayerWon(*players[currect_player_temp]))
	{
		DrawText("You Win!", 200, 200, 20, GREEN);
	}
	else if (!players[currect_player_temp]->IsAlive())
	{
		DrawText("You Lose!", 200, 200, 20, RED);
	}
	else
	{
		for (const auto& rectangle : Wall::GetInstance().GetWall())
			DrawRectangleRec(rectangle, BLACK);

		DrawLine(0, GetScreenWidth(), GetScreenWidth(), GetScreenWidth(), RED);

		players[currect_player_temp]->Draw();

		finish.Draw();
	}
}


void GameScene::onActivate()
{
	factory = new PlayerFactory;

	players.push_back(factory->CreatePlayer());

	Image image_level = LoadImage((path_source + R"(\source\level.png)").c_str());

	try
	{
		Wall::GetInstance().LoadData(image_level);
		players[currect_player_temp]->LoadData(image_level);
		players[currect_player_temp]->SetAlive();
		finish.LoadData(image_level);
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
	delete factory;
	players.clear();
	Wall::GetInstance().Clear();
}
