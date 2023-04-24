#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

#include "Scene.h"
#include "Finish.h"
#include "Enemy.h"
#include "Grid.h"
#include "ButtonFactory.h"


class Player;
class PlayerFactory;

class GameScene : public Scene
{
    Finish finish;
    Grid grid;

    PlayerFactory* playerFactory;
    static Player* player;

    ButtonFactory* buttonFactory;
    std::vector<Button*> buttons;
    size_t buttonExit;

    static std::vector<Enemy> enemies;

    float backToHome;
    float respawnPlayer;
    float timer;

    static bool protected_thread;

public:
    GameScene();

    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;

    static Player* GetPlayer() { return player; }
    static std::vector<Enemy>& GetEnemies() { return enemies; }

    // T - Thread
    static bool T_IsProtectedGameScene();

    static std::mutex GameScene_mutex;
    static std::condition_variable GameScene_cv;
};
