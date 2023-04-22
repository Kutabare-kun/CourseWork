#pragma once
#include "Scene.h"
#include "Player.h"


class Player;

class GameScene : public Scene
{
    Player player;

public:
    GameScene();

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;
};
