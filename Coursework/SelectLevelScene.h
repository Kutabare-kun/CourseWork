#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "Scene.h"
#include "ButtonFactory.h"



class SelectLevelScene :
    public Scene
{
    static std::string pathToLevel;
    Texture2D level;
    std::vector<std::pair<std::string, std::string>> levelName;

    ButtonFactory* buttonFactory;
    std::vector<Button*> buttons;

    TextBox textBox;

    int index;
    int preIndex;

public:
    SelectLevelScene();

    static const std::string& GetLevel() { return pathToLevel; }

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;
};

