#include <iostream>
#include <atomic>
#include <chrono>

#include "SceneManager.h"
#include "GameScene.h"
#include "function.h"

#include <ranges>


using namespace std::chrono_literals;

extern int size_box;

// Thread Console
//--------------------------------------------------------------------------------------
extern std::mutex mtx;
extern std::condition_variable cv;
extern std::atomic<bool> exit_console_game;
extern bool predicate_cond;
//--------------------------------------------------------------------------------------

// Temp
//--------------------------------------------------------------------------------------
extern size_t currect_player_temp;
//--------------------------------------------------------------------------------------


bool operator==(const Color& left, const Color& right)
{
	return left.a == right.a && left.r == right.r && left.g == right.g && left.b == right.b;
}


bool FindColor(const Image& level, const int& StartX, const int& StartY, const int& EndX, const int& EndY, Color color)
{
	for (int x = StartX; x < EndX; ++x)
		for (int y = StartY; y < EndY; ++y)
			if (Color color_pixel = GetImageColor(level, x, y); color_pixel == color)
				return true;

	return false;
}


bool OutOfScreen(const Vector2& pos)
{
	if (pos.x < 0)
		return true;
	else if (pos.x > GetScreenWidth())
		return true;
	else if (pos.y < 0)
		return true;
	else if (pos.y > GetScreenWidth())
		return true;

	return false;
}


void ConsoleThread()
{
	std::string input;

	while (!exit_console_game)
	{
		std::unique_lock<std::mutex> ulm{mtx};
		std::cin.clear();
		cv.wait(ulm, ConsoleOpen);

		if (exit_console_game)
			return;
		
		std::cout << "Enter command: ";
		std::getline(std::cin, input);

		if (!input.empty())
		{
			if (input == "help")
			{
				std::cout << "KillPlayer: \n";
				std::cout << "\ttrue - player is die\n";
				std::cout << "\ttrue - player is alive\n";
				std::cout << "\nClear - clear console\n";
				std::cout << "\nSizeBox - wall square\n";
				std::cout << "\nRestart - restart all level and function\n";
				std::cout << "\nTeleport - teleport player to mouse position\n";
				std::cout << "\nSet Aggression - distance between enemy and player when enemy start aggression\n";
				std::cout << "\nKillEnemy: \n";
				std::cout << "\tAll - kill all enemies\n" << std::endl;
			}
			else if (input.find("KillPlayer") != std::string::npos)
			{
				for (auto value : input | std::ranges::views::split(' ') | std::ranges::views::drop(1))
				{
					std::string command{ value.begin(), value.end() };
					if (command == "true")
						GameScene::GetPlayer()->SetAlive(false);
					else if (command == "false")
						GameScene::GetPlayer()->SetAlive(true);
				}
			}
			else if (input == "Restart")
			{
				std::unique_lock<std::mutex> lock(GameScene::GameScene_mutex);
				GameScene::GameScene_cv.wait(lock, [] { return GameScene::T_IsProtectedGameScene(); });
				SceneManager::GetInstance().Restart();
			}
			else if (input.find("SizeBox") != std::string::npos)
			{
				for (auto value : input | std::ranges::views::split(' ') | std::ranges::views::drop(1))
				{
					bool is_digit{ true };
					std::string command{ value.begin(), value.end() };
					for (const auto& symbol : command)
						if (!std::isdigit(symbol))
						{
							is_digit = false;
							break;
						}

					if (is_digit)
						size_box = std::stoi(command);
					else
					{
						std::cout << "Incorrect command SizeBox!" << std::endl;
						break;
					}
				}
			}
			else if (input == "Teleport")
			{
				Vector2 mousePos = GetMousePosition();
				if (!OutOfScreen(mousePos))
				{
					// Calculate the offset considering player's width and height
					const Rectangle& playerRect = GameScene::GetPlayer()->GetPlayerRect();
					Vector2 offset = { playerRect.width / 2, playerRect.height / 2 }; // Assuming player's width and height are 20

					// Set the player's position with the offset
					GameScene::GetPlayer()->SetPos({ mousePos.x - offset.x, mousePos.y - offset.y });
				}
				else
					std::cout << "Mouse out of Game Screen!" << std::endl;
			}
			else if (input.find("Set Aggression") != std::string::npos)
			{
				std::unique_lock<std::mutex> lock(GameScene::GameScene_mutex);
				GameScene::GameScene_cv.wait(lock, [] { return GameScene::T_IsProtectedGameScene(); });
				for (const auto& count : input | std::ranges::views::split(' ') | std::ranges::views::drop(2))
				{
					bool is_digit{ true };
					std::string command{ count.begin(), count.end() };
					for (const auto& value : command)
						if (!std::isdigit(value))
						{
							is_digit = false;
							break;
						}

					if (is_digit)
						for (auto& enemy : GameScene::GetEnemies())
							enemy.SetAggression(std::stoi(command));
					else
					{
						std::cout << "Incorrect command Set Aggression!" << std::endl;
						break;
					}
				}
			}
			else if (input.find("KillEnemy") != std::string::npos)
			{
				std::unique_lock<std::mutex> lock(GameScene::GameScene_mutex);
				GameScene::GameScene_cv.wait(lock, [] { return GameScene::T_IsProtectedGameScene(); });
				for (auto value : input | std::ranges::views::split(' ') | std::ranges::views::drop(1))
				{
					std::string command{ value.begin(), value.end() };
					if (command == "All")
						GameScene::GetEnemies().clear();
				}
			}
			else if (input == "Clear")
			{
				system("CLS");
			}
			else
			{
				std::cout << "Incorrect command!" << std::endl;
			}

			predicate_cond = false;
		}

		if (input == "exit")
			exit_console_game = true;

		std::this_thread::sleep_for(1s);
	}
}

// Predicate for Console
bool ConsoleOpen()
{
	return predicate_cond;
}
