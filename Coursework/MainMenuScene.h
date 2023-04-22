#pragma once
#include "Scene.h"

class MainMenuScene :
    public Scene
{
public:
    MainMenuScene() = default;

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;
};

