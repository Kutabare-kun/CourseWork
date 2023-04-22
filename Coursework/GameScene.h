#pragma once
#include <vector>

#include "Scene.h"
#include "Finish.h"
#include "Enemy.h"
#include "Grid.h"
#include "Pathfinding.h"


class Player;
class PlayerFactory;

class GameScene : public Scene
{
    Finish finish;
    PlayerFactory* factory;
    static std::vector<Player*> players;
    static std::vector<Enemy> enemies;
    Grid grid;

public:
    GameScene();

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;

    static std::vector<Player*>& GetPlayers() { return players; }
    static std::vector<Enemy> GetEnemies() { return enemies; }
};
