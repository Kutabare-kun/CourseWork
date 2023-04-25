#include <iostream>
#include <ranges>

#include "SceneManager.h"
#include "GameScene.h"
#include "function.h"
#include "SelectLevelScene.h"
#include "Wall.h"
#include "PlayerFactory.h"



// Game Path
//--------------------------------------------------------------------------------------
extern std::string path_source;
//--------------------------------------------------------------------------------------

// Thread Console
//--------------------------------------------------------------------------------------
bool predicate_cond = false;
std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> exit_console_game{ false };
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
	backToHome(5.0f), respawnPlayer(5.0f), timer(0.f),
	gamePause()
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

	if (!finish.PlayerWon(*player) && player->IsAlive())
		for (auto& button : activeButtons)
			button->Update();

	if (!gamePause)
	{
		if (player and !finish.PlayerWon(*player) and player->IsAlive())
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
		else if (player and finish.PlayerWon(*player))
		{
			timer += deltaTime;
			if (timer >= backToHome)
				SceneManager::GetInstance().SetActiveScene(SceneUpdate::MAIN);

			if (buttons.size() > 0)
				buttons[buttonExit]->Update();
		}
		// Player Dead
		else if (player and!player->IsAlive())
		{
			timer += deltaTime;
			if (timer >= respawnPlayer)
				SceneManager::GetInstance().Restart();

			for (auto& button : buttons)
				button->Update();
		}
	}
	//--------------------------------------------------------------------------------------
}


void GameScene::Draw()
{
	// Protect Thread
	//--------------------------------------------------------------------------------------
	std::unique_lock<std::mutex> lock(GameScene_mutex);

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), LIGHTGRAY);

	// Player Won
	if (player and finish.PlayerWon(*player))
	{
		DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
			Color{ 0, 98, 96, 255 }, Color{ 0, 49, 48, 255 });

		DrawText("You Win!", 200, 200, 40, GREEN);
		if (timer < backToHome / 3)
			DrawText(std::to_string(backToHome - timer).c_str(), 200, 250, 16, GREEN);
		else if (timer < backToHome / 3 * 2)
			DrawText(std::to_string(backToHome - timer).c_str(), 200, 250, 16, Color{ 255, 191, 0, 255 });
		else
			DrawText(std::to_string(backToHome - timer).c_str(), 200, 250, 16, RED);

		buttons[buttonExit]->Draw();
	}
	// Player Dead
	else if (player and !player->IsAlive())
	{
		DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
			Color{ 0, 98, 96, 255 }, Color{ 0, 49, 48, 255 });

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
	else if (player)
	{
		for (const auto& rectangle : Wall::GetInstance().GetWall())
			DrawRectangleRec(rectangle, BLACK);

		finish.Draw();

		player->Draw();

		for (const auto& enemy : enemies)
			enemy.Draw();

		for (const auto& button : activeButtons)
			button->Draw();

		if (gamePause)
		{
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenWidth(), Color{ 0, 0, 0, 70 });
			DrawText("PAUSE", 250, 250, 25, WHITE);
		}
	}

	//--------------------------------------------------------------------------------------

	protected_thread = true;
	lock.unlock();

	GameScene_cv.notify_all();
}


void GameScene::onActivate()
{
	gamePause = false;

	// Init Buttons
	//--------------------------------------------------------------------------------------
	buttonFactory = new ButtonFactory;

	float CenterX = GetScreenWidth() / 2;
	float width = 200;

	// Buttons during Win\Lose Conditions
	//--------------------------------------------------------------------------------------
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
			[]()
			{
				SceneManager::GetInstance().Restart();
			},
			{CenterX - width / 2, 350, width, 50},
			"Restart", 20, Color{ 0, 49, 48, 255 },
			Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
		)
	);
	// Buttons during the game
	//--------------------------------------------------------------------------------------
	activeButtons.push_back(
		buttonFactory->CreateButton(
			[this]()
			{
				gamePause = !gamePause;
			},
			{ 0, 576, static_cast<float>(GetScreenWidth() / 3), 48 },
				"Pause", 20, Color{ 0, 49, 48, 255 },
				Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
		)
	);

	activeButtons.push_back(
		buttonFactory->CreateButton(
			SceneUpdate::MAIN,
			{ static_cast<float>(GetScreenWidth() - GetScreenWidth() / 3), 576, static_cast<float>(GetScreenWidth() / 3), 48 },
				"Main Menu", 20, Color{ 0, 49, 48, 255 },
				Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
				)
	);

	activeButtons.push_back(
		buttonFactory->CreateButton(
			[]()
			{
				SceneManager::GetInstance().Restart();
			},
			{ static_cast<float>(GetScreenWidth() / 3), 576, static_cast<float>(GetScreenWidth() / 3), 48 },
				"Restart", 20, Color{ 0, 49, 48, 255 },
				Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
				)
	);
	//--------------------------------------------------------------------------------------

	// Init Game Area to grid
	//--------------------------------------------------------------------------------------
	grid.Init(GetScreenWidth() / size_box, GetScreenWidth() / size_box, size_box);
	//--------------------------------------------------------------------------------------

	// Init Players, enemies and walls locations
	//--------------------------------------------------------------------------------------
	playerFactory = new PlayerFactory;

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
	//--------------------------------------------------------------------------------------

	// Open thread
	//--------------------------------------------------------------------------------------
	consoleThread = std::thread{ ConsoleThread };
	consoleThread.detach();

	predicate_cond = false;
	exit_console_game = false;
	//--------------------------------------------------------------------------------------
}


void GameScene::onDeactivate()
{
	delete playerFactory;
	delete buttonFactory;
	player = nullptr;
	buttons.clear();
	activeButtons.clear();
	enemies.clear();
	grid.Clear();
	Wall::GetInstance().Clear();


	// Turn off thread
	//--------------------------------------------------------------------------------------
	exit_console_game = true;
	predicate_cond = true;
	cv.notify_one();

	// Safe close
	//--------------------------------------------------------------------------------------
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1ms);
	//--------------------------------------------------------------------------------------
}


bool GameScene::T_IsProtectedGameScene()
{
	return protected_thread;
}
