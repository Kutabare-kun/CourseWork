#include "BuildLevelScene.h"

#include <iostream>
#include <string>
#include <filesystem>


// Global variables
//--------------------------------------------------------------------------------------
extern int size_box;
//--------------------------------------------------------------------------------------

// Game Path
//--------------------------------------------------------------------------------------
extern std::string path_source;
//--------------------------------------------------------------------------------------


void BuildLevelScene::Update(float deltaTime)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 MousePos{ GetMousePosition() };
        if (MousePos.x >= 0 && MousePos.x <= GetScreenWidth()
            && MousePos.y >= 0 && MousePos.y <= GetScreenWidth())
        {
            Vector2 MousePosGrid{ grid.WorldToGrid(MousePos.x, MousePos.y) };

            if (Flag & static_cast<int>(PaintFlag::P_PAINT))
            {
                int flag = Flag & (static_cast<int>(PaintFlag::P_WALL) | static_cast<int>(PaintFlag::P_DELETE) |
                    static_cast<int>(PaintFlag::P_START) | static_cast<int>(PaintFlag::P_FINISH));

                switch (flag)
                {
                case static_cast<int>(PaintFlag::P_WALL):
                    grid.SetWalkable(MousePosGrid.y, MousePosGrid.x, false);
                    break;
                case static_cast<int>(PaintFlag::P_DELETE):
                    grid.SetWalkable(MousePosGrid.y, MousePosGrid.x, true);
                    break;
                case static_cast<int>(PaintFlag::P_START):
                    FromTo.first = MousePosGrid;
                    grid.SetWalkable(MousePosGrid.y, MousePosGrid.x, true);
                    break;
                case static_cast<int>(PaintFlag::P_FINISH):
                    FromTo.second = MousePosGrid;
                    grid.SetWalkable(MousePosGrid.y, MousePosGrid.x, true);
                    break;
                }

                WallPos.clear();

                const auto& wall = grid.GetGrid();
                for (int x = 0; x < wall.size(); ++x)
                {
                    const float xPos = x * size_box;
                    for (int y = 0; y < wall[x].size(); ++y)
                        if (!wall[x][y])
                            WallPos.push_back({ xPos, static_cast<float>(y * size_box) });
                }

                path.clear();

                if (Save)
                {
	                Save = false;
	                DelayToScreenshot = 0.f;
                }
            }
        }
    }
    else if (Flag & static_cast<int>(PaintFlag::P_WATCH))
        path = Pathfinding::FindPath(grid, { FromTo.first.y, FromTo.first.x }, { FromTo.second.y, FromTo.second.x });

    if (Save && Timer == 0.f && !path.empty())
    {
        std::cout << "Enter File Name: ";
        std::cin.clear();
        std::getline(std::cin, fileName);
        fileName += ".png";
        Timer += deltaTime;
        Save = false;

        path.clear();
    }
    else if (Timer > 0.f && Timer < DelayToScreenshot)
        Timer += deltaTime;
    else if (Timer >= DelayToScreenshot)
    {
        Timer = 0.f;
        TakeScreenshot(fileName.c_str());

        std::filesystem::rename(std::filesystem::current_path() / fileName, std::filesystem::path(path_source) / "source" / fileName);

        fileName.clear();
    }
    else if (Save && path.empty())
    {
        Save = !Save;
        std::cout << "Couldn't find the successful path" << std::endl;
    }

    for (auto& button : buttons)
        button->Update();
}


void BuildLevelScene::Draw()
{
    const auto& wall = grid.GetGrid();

    DrawRectangle(FromTo.first.x * size_box, FromTo.first.y * size_box, size_box, size_box, BLUE);
    DrawRectangle(FromTo.second.x * size_box, FromTo.second.y * size_box, size_box, size_box, ORANGE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (!path.empty())
        for (const auto& vector2 : path)
            DrawRectangleV({vector2.y, vector2.x }, { static_cast<float>(size_box), static_cast<float>(size_box) }, LIME);

    for (const auto& wall : WallPos)
        DrawRectangleV(wall, { static_cast<float>(size_box), static_cast<float>(size_box) }, BLACK);

    if (Timer == 0.f)
    {
	    for (int x = 0; x < 18; ++x)
	        DrawLine(x * size_box, 0, x * size_box, screenHeight, BLACK);
	    for (int y = 0; y < 18; ++y)
	        DrawLine(0, y * size_box, screenWidth, y * size_box, BLACK);
    }

    for (const auto& button : buttons)
        button->Draw();
}


void BuildLevelScene::onActivate()
{
	Flag |= static_cast<int>(PaintFlag::P_NONE);
	grid.Init(GetScreenWidth() / size_box, GetScreenWidth() / size_box, size_box);
	FromTo = { {0,0}, {17,17} };

    buttonFactory = new ButtonFactory;

    float ScreenHeight = GetScreenHeight();
    float width = 108;
    float widthButton = 78;
    float height = 50;

    buttons.push_back(
        buttonFactory->CreateButton(
            [this]()
            {
                Flag = static_cast<int>(PaintFlag::P_NONE);
                Flag |= static_cast<int>(PaintFlag::P_PAINT) | static_cast<int>(PaintFlag::P_DELETE);
            },
            { 0, ScreenHeight - height, widthButton, height },
            "delete", 20, Color{ 0, 49, 48, 255 },
            Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
        )
    );

    buttons.push_back(
        buttonFactory->CreateButton(
            [this]()
            {
                Flag = static_cast<int>(PaintFlag::P_NONE);
                Flag |= static_cast<int>(PaintFlag::P_PAINT) | static_cast<int>(PaintFlag::P_WALL);
            },
            { widthButton, ScreenHeight - height, widthButton, height },
                "wall", 20, Color{ 0, 49, 48, 255 },
                Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
                )
    );

    buttons.push_back(
        buttonFactory->CreateButton(
            [this]()
            {
                Flag = static_cast<int>(PaintFlag::P_NONE);
                Flag |= static_cast<int>(PaintFlag::P_WATCH);
            },
            { widthButton * 2, ScreenHeight - height, widthButton, height },
                "calcul", 20, Color{ 0, 49, 48, 255 },
                Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
                )
    );

    buttons.push_back(
        buttonFactory->CreateButton(
            [this]()
            {
                Flag = static_cast<int>(PaintFlag::P_NONE);
                Flag |= static_cast<int>(PaintFlag::P_PAINT) | static_cast<int>(PaintFlag::P_START);
            },
            { widthButton * 3, ScreenHeight - height, widthButton, height },
                "start", 20, Color{ 0, 49, 48, 255 },
                Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
                )
    );

    buttons.push_back(
        buttonFactory->CreateButton(
            [this]()
            {
                Flag = static_cast<int>(PaintFlag::P_NONE);
                Flag |= static_cast<int>(PaintFlag::P_PAINT) | static_cast<int>(PaintFlag::P_FINISH);
            },
            { widthButton * 4, ScreenHeight - height, widthButton, height },
                "finish", 20, Color{ 0, 49, 48, 255 },
                Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
                )
    );

    buttons.push_back(
        buttonFactory->CreateButton(
            [this]()
            {
                Save = true;
            },
            { widthButton * 5, ScreenHeight - height, widthButton, height },
                "save", 20, Color{ 0, 49, 48, 255 },
                Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
                )
    );

    buttons.push_back(
        buttonFactory->CreateButton(
            SceneUpdate::MAIN,
            { GetScreenWidth() - width, ScreenHeight - height, width, height },
            "Exit", 20, Color{ 0, 49, 48, 255 },
            Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
        )
    );

    Save = false;
    Timer = 0.f;
    DelayToScreenshot = 5.f;
}


void BuildLevelScene::onDeactivate()
{
    delete buttonFactory;
    buttons.clear();
    WallPos.clear();
    path.clear();
    Flag = static_cast<int>(PaintFlag::P_NONE);
    Save = false;
    Timer = 0.f;
}
