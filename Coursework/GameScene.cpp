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

//
//--------------------------------------------------------------------------------------
extern int size_box;
//--------------------------------------------------------------------------------------


// Temp
//--------------------------------------------------------------------------------------
size_t currect_player_temp = 0;
//--------------------------------------------------------------------------------------


std::vector<Player*> GameScene::players;
std::vector<Enemy> GameScene::enemies;


GameScene::GameScene()
	: finish()
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

	Rectangle posPlayer = players[currect_player_temp]->GetPlayerRect();
	for (auto& enemy : enemies)
		enemy.SetTarget({ std::ceil(posPlayer.x + posPlayer.width / 2), std::ceil(posPlayer.y + posPlayer.height / 2) });

	for (auto & enemy : enemies)
		enemy.Update(deltaTime);
}


void GameScene::Draw()
{
	if (finish.PlayerWon(*players[currect_player_temp]))
	{
		DrawText("You Win!", 200, 200, 30, GREEN);
	}
	//else if (!players[currect_player_temp]->IsAlive())
	//{
	//	DrawText("You Lose!", 200, 200, 30, RED);
	//}
	else
	{
		for (const auto& rectangle : Wall::GetInstance().GetWall())
			DrawRectangleRec(rectangle, BLACK);

		DrawLine(0, GetScreenWidth(), GetScreenWidth(), GetScreenWidth(), RED);

		players[currect_player_temp]->Draw();

		finish.Draw();

		for (const auto & enemy : enemies)
			enemy.Draw();
	}
}


void GameScene::onActivate()
{
	factory = new PlayerFactory;

	grid.Init(GetScreenWidth() / size_box, GetScreenWidth() / size_box, size_box);

	players.push_back(factory->CreatePlayer());

	Image image_level = LoadImage((path_source + R"(\source\level.png)").c_str());

	try
	{
		Wall::GetInstance().LoadData(image_level);
		players[currect_player_temp]->LoadData(image_level);
		players[currect_player_temp]->SetAlive();

		for (const auto& source : Enemy::LoadData(image_level))
		{
			Enemy enemy;
			enemy.Init(source, &grid, 300);
			enemies.push_back(enemy);
		}

		finish.LoadData(image_level);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	enemies.shrink_to_fit();
	grid.UpdateWalkableWithWalls(Wall::GetInstance().GetWall());

	UnloadImage(image_level);

	Wall::GetInstance().MergeCloseRectangles();
}


void GameScene::onDeactivate()
{
	delete factory;
	players.clear();
	enemies.clear();
	Wall::GetInstance().Clear();
}
