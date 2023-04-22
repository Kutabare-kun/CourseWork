#include "PlayerFactory.h"

Player* PlayerFactory::CreatePlayer()
{
	auto player = std::make_unique<Player>();
	Player* playerPtr = player.get();
	players.push_back(std::move(player));
	return playerPtr;
}
