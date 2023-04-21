#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "function.h"
#include "Player.h"
#include "Wall.h"


bool predicate_cond = false;

std::string path_source{ std::filesystem::current_path().string().erase(std::filesystem::current_path().string().find_last_of('\\')) };

std::mutex mtx;
std::condition_variable cv;

std::atomic<bool> exit_console_game{ false };


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 576;
    const int screenHeight = 624;

    InitWindow(screenWidth, screenHeight, "Maze");

    SetTargetFPS(60);

    Image image_level = LoadImage((path_source + R"(\source\level.png)").c_str());

    try
    {
        Wall::LoadData(image_level);
        Wall::MergeCloseRectangles();
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    UnloadImage(image_level);

    std::thread consoleThread(ConsoleThread);

    Player player{ 5, 5, 20, 20, 500 };

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_GRAVE))
        {
            exit_console_game = false;
            predicate_cond = !predicate_cond;
        	cv.notify_one();
        }

        player.Update(GetFrameTime());
    	//----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const auto& rectangle : Wall::get_wall())
	        if (rectangle.width > 64)
                DrawRectangleRec(rectangle, RED);
            else if (rectangle.height > 64)
				DrawRectangleRec(rectangle, BLACK);

        player.Draw();

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
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}