#pragma once
#include <vector>

#include "Scene.h"
#include "Finish.h"

class Player;
class PlayerFactory;

class GameScene : public Scene
{
    std::vector<Player*> players;
    Finish finish;
    PlayerFactory* factory;

public:
    GameScene();

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;
};
