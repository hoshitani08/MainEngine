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

void SceneManager::Initialize()
{
	if (scene_)
	{
		scene_->Finalize();
	}
	scene_ = std::unique_ptr<BaseScene>(nextScene_);
	scene_->Initialize();

	loadType = LoadType::LoadEnd;
}

void SceneManager::Finalize()
{
	scene_->Finalize();
	scene_ = nullptr;

	loadScene_->Finalize();
	loadScene_ = nullptr;
}

void SceneManager::Update()
{
	if (nextScene_)
	{
		if (!loadFlag)
		{
			std::thread tmp = std::thread([&] { Initialize(); });
			t.swap(tmp);
		}

		switch (loadType)
		{
		case SceneManager::LoadType::NoLoad: // ���[�h�n�܂��ĂȂ�
			loadFlag = true;
			// ���[�h���=���[�h�n�܂���
			loadType = LoadType::LoadStart;
			break;
		case SceneManager::LoadType::LoadStart:
			break;
		case SceneManager::LoadType::LoadEnd:
			t.join();
			// ���[�h�̏I��
			nextScene_ = nullptr;
			loadFlag = false;
			// ���=���[�h���
			loadType = LoadType::NoLoad;
			break;
		default:
			break;
		}
	}
	
	if (!loadFlag)
	{
		scene_->Update();
	}
	else
	{
		loadScene_->Update();
	}
}

void SceneManager::Draw()
{
	if (!loadFlag)
	{
		scene_->Draw();
	}
	else
	{
		loadScene_->Draw();
	}
}

void SceneManager::EffectDraw()
{
	if (!loadFlag)
	{
		scene_->EffectDraw();
	}
	else
	{
		loadScene_->EffectDraw();
	}
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(sceneName);
}

void SceneManager::SetLoadScene(const std::string& sceneName)
{
	loadScene_ = std::unique_ptr<BaseScene>(sceneFactory_->CreateScene(sceneName));
	loadScene_->Initialize();
}