#pragma once
#include "Pathfinding.h"
#include "Scene.h"
#include "ButtonFactory.h"


class BuildLevelScene
	: public Scene
{
    Grid grid;
    std::pair<Vector2, Vector2> FromTo;
    std::vector<Vector2> WallPos;

    std::vector<Vector2> path;

    ButtonFactory* buttonFactory;
    std::vector<Button*> buttons;

    int Flag : 6;

public:
    void Update(float deltaTime) override;
    void Draw() override;
    void onActivate() override;
    void onDeactivate() override;
};

