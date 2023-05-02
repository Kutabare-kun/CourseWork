#include "BuildLevelScene.h"

#include <iostream>


// Global variables
//--------------------------------------------------------------------------------------
extern int size_box;
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
            }
        }
    }
    else if (Flag & static_cast<int>(PaintFlag::P_WATCH))
        path = Pathfinding::FindPath(grid, { FromTo.first.y, FromTo.first.x }, { FromTo.second.y, FromTo.second.x });

    for (auto& button : buttons)
        button->Update();
}


void BuildLevelScene::Draw()
{
    const auto& wall = grid.GetGrid();

    DrawRectangle(FromTo.first.x * size_box, FromTo.first.y * size_box, size_box, size_box, ORANGE);
    DrawRectangle(FromTo.second.x * size_box, FromTo.second.y * size_box, size_box, size_box, RED);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (!path.empty())
        for (const auto& vector2 : path)
            DrawRectangleV({vector2.y, vector2.x}, { static_cast<float>(size_box), static_cast<float>(size_box) }, LIME);

    for (const auto& wall : WallPos)
        DrawRectangleV(wall, { static_cast<float>(size_box), static_cast<float>(size_box) }, BLACK);

    for (int x = 0; x < 18; ++x)
        DrawLine(x * size_box, 0, x * size_box, screenHeight, BLACK);
    for (int y = 0; y < 18; ++y)
        DrawLine(0, y * size_box, screenWidth, y * size_box, BLACK);

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
    float width = 186;
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
            SceneUpdate::MAIN,
            { GetScreenWidth() - width, ScreenHeight - height, width, height },
            "Exit", 20, Color{ 0, 49, 48, 255 },
            Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
        )
    );
}


void BuildLevelScene::onDeactivate()
{
    delete buttonFactory;
    buttons.clear();
    WallPos.clear();
    path.clear();
    Flag = static_cast<int>(PaintFlag::P_NONE);
}
