#include "MainMenuScene.h"


extern bool GameExit;


void MainMenuScene::Update(float deltaTime)
{
	for (auto & button : buttons)
		button->Update();
}


void MainMenuScene::Draw()
{
	DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
		Color{ 0, 98, 96, 255 }, Color{ 0, 49, 48, 255 });

	for (const auto & button : buttons)
		button->Draw();
}


void MainMenuScene::onActivate()
{
	buttonFactory = new ButtonFactory;

	float CenterX = GetScreenWidth() / 2;
	float CenterY = GetScreenHeight() / 2;
	float width = 200;

	// Start
	buttons.push_back(buttonFactory->CreateButton(
		SceneUpdate::SELECTLEVEL,
		{ CenterX - width / 2, CenterY - 50, width, 50 },
		"START", 20, Color{0, 49, 48, 255}, 
		Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
	));

	buttons.push_back(buttonFactory->CreateButton(
		[]()
		{
			GameExit = true;
		},
		{ CenterX - width / 2, CenterY + 50, width, 50 },
		"EXIT", 20, Color{ 0, 49, 48, 255 },
		Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
	));
}


void MainMenuScene::onDeactivate()
{
	delete buttonFactory;
	buttons.clear();
}
