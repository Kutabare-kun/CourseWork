#include "SceneManager.h"
#include "GameScene.h"
#include "MainMenuScene.h"

SceneManager::SceneManager() : scenes(), current_scene(0)
{}


SceneManager& SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}

void SceneManager::Init()
{
	AddScene(new MainMenuScene);
	AddScene(new GameScene);
}


void SceneManager::AddScene(Scene* scene)
{
	scenes.emplace_back(scene);
}


void SceneManager::SetActiveScene(SceneUpdate index)
{
	scenes[current_scene]->onDeactivate();
	current_scene = static_cast<size_t>(index);
	scenes[current_scene]->onActivate();
}


void SceneManager::Update(float deltaTime)
{
    scenes[current_scene]->Update(deltaTime);
}

void SceneManager::Draw()
{
	scenes[current_scene]->Draw();
}
