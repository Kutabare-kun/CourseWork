#pragma once
#include <vector>
#include <memory>
#include "Player.h"

class PlayerFactory
{
private:
	std::vector<std::unique_ptr<Player>> players;

public:
	Player* CreatePlayer();
};
