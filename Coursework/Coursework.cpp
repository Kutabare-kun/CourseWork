#include <iostream>
#include <format>

#include "function.h"
#include "math.h"


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 576;
    const int screenHeight = 624;

    InitWindow(screenWidth, screenHeight, "Maze");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Image image_level = LoadImage("C:\\Users\\mrsmi\\GitHub\\CourseWork\\source\\level.png");

    auto wall = GetImageWall(image_level);

    UnloadImage(image_level);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto rectangle : wall)
            DrawRectangleRec(rectangle, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}