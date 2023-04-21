#include <iostream>
#include <format>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "function.h"
#include "Wall.h"
#include "math.h"


bool predicat_cond = false;

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

    Image image_level = LoadImage("C:\\Users\\mrsmi\\GitHub\\CourseWork\\source\\level.png");

    try
    {
        Wall::LoadData(image_level);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    UnloadImage(image_level);

    std::thread consoleThread(ConsoleThread);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_GRAVE))
        {
            exit_console_game = false;
            predicat_cond = !predicat_cond;
        	cv.notify_one();
        }
    	//----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const auto& rectangle : Wall::get_wall())
            DrawRectangleRec(rectangle, BLACK);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // Turn off thread
	//--------------------------------------------------------------------------------------
    exit_console_game = true;
    predicat_cond = true;
    cv.notify_one();
    consoleThread.join();
    //--------------------------------------------------------------------------------------


    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}