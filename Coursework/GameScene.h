#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

#include "Scene.h"
#include "Finish.h"
#include "Enemy.h"
#include "Grid.h"


class Player;
class PlayerFactory;

class GameScene : public Scene
{
    Finish finish;
    PlayerFactory* factory;
    static std::vector<Player*> players;
    static std::vector<Enemy> enemies;
    Grid grid;

    static bool protected_thread;

public:
    GameScene();

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;

    static std::vector<Player*>& GetPlayers() { return players; }
    static std::vector<Enemy>& GetEnemies() { return enemies; }

    // T - Thread
    static bool T_IsProtectedGameScene();

    static std::mutex GameScene_mutex;
    static std::condition_variable GameScene_cv;
};
