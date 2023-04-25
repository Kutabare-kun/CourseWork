#pragma once
#include "Scene.h"
#include "ButtonFactory.h"

class MainMenuScene :
    public Scene
{
private:
    ButtonFactory* buttonFactory;
    std::vector<Button*> buttons;

public:
    MainMenuScene() = default;

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;
};

