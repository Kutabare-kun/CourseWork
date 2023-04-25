#pragma once
#include <raylib.h>

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

	void Update(float delta);
	void Draw();

	void SetPos(const Vector2& pos);

	void SetAlive();
	void SetAlive(bool alive);
	const bool& IsAlive() const { return alive; }

	const Rectangle& GetPlayerRect() const { return playerRect; }
};
