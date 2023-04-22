#pragma once
#include <raylib.h>
#include <string>


class Player
{
private:
	Rectangle playerRect;
	Vector2 velocity;
	float speed;

	bool alive;

	void ResolveCollision(const Rectangle& wall);

public:
	Player() = default;

	void LoadData(const Image& level);
	void LoadData(const std::string& path);

	void Update(float delta);
	void Draw();

	void SetAlive();
	const bool& IsAlive() const { return alive; }

	const Rectangle& GetPlayerRect() const { return playerRect; }
};
