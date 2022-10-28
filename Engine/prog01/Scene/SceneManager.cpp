#include "SceneManager.h"

#include <cassert>

SceneManager::~SceneManager()
{
	if (scene_)
	{
		Finalize();
	}
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::Finalize()
{
	scene_->Finalize();
	delete scene_;
	scene_ = nullptr;
}

void SceneManager::Update()
{
	if (nextScene_)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}
		scene_ = nextScene_;
		nextScene_ = nullptr;

		scene_->Initialize();
	}
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::EffectDraw()
{
	scene_->EffectDraw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(sceneName);
}