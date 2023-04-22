#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "function.h"
#include "SceneManager.h"


// Window Parameters
//--------------------------------------------------------------------------------------
const int screenWidth = 576;
const int screenHeight = 624;
//--------------------------------------------------------------------------------------

// Size Box
//--------------------------------------------------------------------------------------
int size_box{ 32 };
//--------------------------------------------------------------------------------------


// Game Path
//--------------------------------------------------------------------------------------
std::string path_source{
	std::filesystem::current_path().string().erase(std::filesystem::current_path().string().find_last_of('\\'))
};
//--------------------------------------------------------------------------------------

// Thread Console
//--------------------------------------------------------------------------------------
bool predicate_cond = false;
std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> exit_console_game{false};
//--------------------------------------------------------------------------------------


int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "Maze");

	std::thread consoleThread(ConsoleThread);

	SceneManager::GetInstance().Init();
	SceneManager::GetInstance().SetActiveScene(SceneUpdate::GAME);

	SetTargetFPS(144);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())
	{
		// Update
		//----------------------------------------------------------------------------------

		SceneManager::GetInstance().Update(GetFrameTime());
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(RAYWHITE);

		SceneManager::GetInstance().Draw();

		DrawFPS(10, 10);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// Turn off thread
	//--------------------------------------------------------------------------------------
	exit_console_game = true;
	predicate_cond = true;
	cv.notify_one();
	consoleThread.join();
	//--------------------------------------------------------------------------------------


	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}
