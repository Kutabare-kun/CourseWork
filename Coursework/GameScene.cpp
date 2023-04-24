#include <iostream>
#include <string>
#include <condition_variable>
#include <atomic>
#include <ranges>

#include "SceneManager.h"
#include "GameScene.h"
#include "SelectLevelScene.h"
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

// Global variables
//--------------------------------------------------------------------------------------
extern int size_box;
//--------------------------------------------------------------------------------------


// Initialization GameScene
//--------------------------------------------------------------------------------------
// Player and Enemies
//--------------------------------------------------------------------------------------
Player* GameScene::player;
std::vector<Enemy> GameScene::enemies;
// Thread
//--------------------------------------------------------------------------------------
bool GameScene::protected_thread{ false };
std::mutex GameScene::GameScene_mutex;
std::condition_variable GameScene::GameScene_cv;
//--------------------------------------------------------------------------------------


GameScene::GameScene()
	: finish(), buttonExit(),
	backToHome(5.0f), respawnPlayer(5.0f), timer(0.f)
{}


void GameScene::Update(float deltaTime)
{
	if (IsKeyDown(KEY_GRAVE))
	{
		exit_console_game = false;
		predicate_cond = !predicate_cond;
		cv.notify_one();
	}

	// Protect Thread
	//--------------------------------------------------------------------------------------
	protected_thread = false;
	std::unique_lock<std::mutex> lock(GameScene_mutex);

	if (!finish.PlayerWon(*player) and player->IsAlive())
	{
		player->Update(deltaTime);

		Rectangle posPlayer = player->GetPlayerRect();
		for (auto& enemy : enemies)
		{
			enemy.SetTarget(posPlayer);
			enemy.Update(deltaTime);
			player->SetAlive(!CheckCollisionRecs(player->GetPlayerRect(), enemy.GetEnemyRect()));
		}

		// Reset timer
		timer = 0.f;
	}
	// Player Won
	else if (finish.PlayerWon(*player))
	{
		timer += deltaTime;
		if (timer >= backToHome)
			SceneManager::GetInstance().SetActiveScene(SceneUpdate::MAIN);

		buttons[buttonExit]->Update();
	}
	// Player Dead
	else if (!player->IsAlive())
	{
		timer += deltaTime;
		if (timer >= respawnPlayer)
			SceneManager::GetInstance().SetActiveScene(SceneUpdate::GAME);

		for (auto& button : buttons)
			button->Update();
	}
	//--------------------------------------------------------------------------------------
}


void GameScene::Draw()
{
	// Protect Thread
	//--------------------------------------------------------------------------------------
	std::unique_lock<std::mutex> lock(GameScene_mutex);

	// Player Won
	if (finish.PlayerWon(*player))
	{
		DrawText("You Win!", 200, 200, 40, GREEN);
		if (timer < backToHome / 3)
			DrawText(std::to_string(backToHome - timer).c_str(), 200, 250, 16, GREEN);
		else if (timer < backToHome / 3 * 2)
			DrawText(std::to_string(backToHome - timer).c_str(), 200, 250, 16, Color{ 255, 191, 0, 255});
		else
			DrawText(std::to_string(backToHome - timer).c_str(), 200, 250, 16, RED);

		buttons[buttonExit]->Draw();
	}
	// Player Dead
	else if (!player->IsAlive())
	{
		DrawText("You Lose!", 200, 200, 30, RED);
		if (timer < respawnPlayer / 3)
			DrawText(std::to_string(respawnPlayer - timer).c_str(), 200, 250, 16, GREEN);
		else if (timer < respawnPlayer / 3 * 2)
			DrawText(std::to_string(respawnPlayer - timer).c_str(), 200, 250, 16, Color{ 255, 191, 0, 255 });
		else
			DrawText(std::to_string(respawnPlayer - timer).c_str(), 200, 250, 16, RED);

		for (const auto& button : buttons | std::ranges::views::reverse)
			button->Draw();
	}
	// Game
	else
	{
		for (const auto& rectangle : Wall::GetInstance().GetWall())
			DrawRectangleRec(rectangle, BLACK);

		DrawLine(0, GetScreenWidth(), GetScreenWidth(), GetScreenWidth(), RED);

		finish.Draw();

		player->Draw();

		for (const auto & enemy : enemies)
			enemy.Draw();
	}
	//--------------------------------------------------------------------------------------

	protected_thread = true;
	lock.unlock();

	GameScene_cv.notify_all();
}


void GameScene::onActivate()
{
	buttonFactory = new ButtonFactory;

	float CenterX = GetScreenWidth() / 2;
	float width = 200;

	buttons.push_back(
		buttonFactory->CreateButton(
			SceneUpdate::MAIN,
			{ CenterX - width / 2, 425, width, 50 },
			"Exit", 20, Color{ 0, 49, 48, 255 },
			Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
		)
	);

	buttons.push_back(
		buttonFactory->CreateButton(
			SceneUpdate::GAME, 
			{CenterX - width / 2, 350, width, 50}, 
			"Restart", 20, Color{ 0, 49, 48, 255 },
			Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
		)
	);
	
	playerFactory = new PlayerFactory;

	grid.Init(GetScreenWidth() / size_box, GetScreenWidth() / size_box, size_box);

	player = playerFactory->CreatePlayer();

	Image image_level = LoadImage(SelectLevelScene::GetLevel().c_str());

	try
	{
		Wall::GetInstance().LoadData(image_level);
		player->LoadData(image_level);
		player->SetAlive();

		for (const auto& source : Enemy::LoadData(image_level))
		{
			Enemy enemy;
			enemy.Init(source, &grid, 300, 7);
			enemies.push_back(enemy);
		}

		finish.LoadData(image_level);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	UnloadImage(image_level);

	enemies.shrink_to_fit();
	grid.UpdateWalkableWithWalls(Wall::GetInstance().GetWall());

	Wall::GetInstance().MergeCloseRectangles();
}


void GameScene::onDeactivate()
{
	delete playerFactory;
	delete buttonFactory;
	player = nullptr;
	buttons.clear();
	enemies.clear();
	grid.Clear();
	Wall::GetInstance().Clear();
}


bool GameScene::T_IsProtectedGameScene()
{
	return protected_thread;
}
