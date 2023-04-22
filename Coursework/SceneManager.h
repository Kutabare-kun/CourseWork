#pragma once
#include <vector>
#include <memory>

#include "Scene.h"
#include "Enums.h"

class SceneManager final
{
private:
	std::vector<std::unique_ptr<Scene>> scenes;
	size_t current_scene;

	
	SceneManager();

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	void AddScene(Scene* scene);

public:

	void Init();
	static SceneManager& GetInstance();
	void SetActiveScene(SceneUpdate index);
	void Update(float deltaTime);
	void Draw();
};

