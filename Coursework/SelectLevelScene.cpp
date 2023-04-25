#include "SelectLevelScene.h"

extern std::string path_source;
std::string SelectLevelScene::pathToLevel;


SelectLevelScene::SelectLevelScene()
	: buttonFactory(nullptr), textBox({}, "", 25, Color{ 0, 49, 48, 255 }), index()
{}


void SelectLevelScene::Update(float deltaTime)
{
	for (auto & button : buttons)
		button->Update();

	if (index != preIndex)
	{
		preIndex = index;

		UnloadTexture(level);
		Image image = LoadImage((pathToLevel = (path_source + "\\source\\" + levelName[index].first)).c_str());
		ImageResize(&image, 400, 400);
		level = LoadTextureFromImage(image);
		UnloadImage(image);

		textBox.SetText(levelName[index].second);
		float CenterX = GetScreenWidth() / 2;
		Vector2 pos{ CenterX - TextBox::CalculateWidthText(levelName[index].second.c_str()) / 2,25 };
		textBox.SetPos(pos);
	}
}


void SelectLevelScene::Draw()
{
	DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
		Color{ 0, 179, 177, 255 }, Color{ 0, 49, 48, 255 });

	float CenterX = GetScreenWidth() / 2;
	DrawTexture(level, CenterX - level.width / 2, 50, WHITE);

	for (const auto & button : buttons)
		button->Draw();

	textBox.Draw();
}


void SelectLevelScene::onActivate()
{
	levelName.emplace_back("level.png", "Easy");
	levelName.emplace_back("level_1.png", "Medium");
	//levelName.emplace_back("level_2.png", "Hard");

	Image image = LoadImage((pathToLevel = (path_source + "\\source\\" + levelName.at(index).first)).c_str());
	ImageResize(&image, 400, 400);
	level = LoadTextureFromImage(image);
	UnloadImage(image);

	float CenterX = GetScreenWidth() / 2;
	float width = 200;

	buttonFactory = new ButtonFactory;

	buttons.push_back(buttonFactory->CreateButton(
		SceneUpdate::GAME,
		{ CenterX - width / 2, 475, width, 50},
		"Play", 20, Color{ 0, 49, 48, 255 },
		Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
	));

	buttons.push_back(buttonFactory->CreateButton(
		SceneUpdate::MAIN,
		{ CenterX - width / 2, 550, width, 50 },
		"Exit", 20, Color{ 0, 49, 48, 255 },
		Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
	));

	buttons.push_back(buttonFactory->CreateButton(
		[this]()
		{
			index = --index >= 0 ? index : index + 1;
		},
		{ 25, 150, 50, width },
		"<", 40, Color{ 0, 49, 48, 255 },
		Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
	));

	buttons.push_back(buttonFactory->CreateButton(
		[this]()
		{
			index = ++index < levelName.size() ? index : index - 1;
		},
		{ static_cast<float>(GetScreenWidth() - 75), 150, 50, width },
		">", 40, Color{ 0, 49, 48, 255 },
		Color{ 0, 149, 146, 255 }, Color{ 0, 49, 48, 50 }
	));

	textBox.SetText(levelName[index].second);
	Vector2 pos{ CenterX - TextBox::CalculateWidthText(textBox.GetText().c_str()) / 2,25 };
	textBox.SetPos(pos);

	preIndex = index;
}


void SelectLevelScene::onDeactivate()
{
	delete buttonFactory;
	UnloadTexture(level);
	levelName.clear();
	buttons.clear();
}
