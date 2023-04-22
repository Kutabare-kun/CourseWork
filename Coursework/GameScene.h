#pragma once
#include <vector>

#include "Scene.h"
#include "Finish.h"
#include "Enemy.h"

class Player;
class PlayerFactory;

class GameScene : public Scene
{
    static std::vector<Player*> players;
    static std::vector<Enemy*> enemies;
    Finish finish;
    PlayerFactory* factory;

public:
    GameScene();

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;

    static std::vector<Player*>& GetPlayers() { return players; }
    static std::vector<Enemy*> GetEnemies() { return enemies; }
};
