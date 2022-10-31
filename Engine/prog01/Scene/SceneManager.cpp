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

	loadType_ = LoadType::LoadEnd;
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
		switch (loadType_)
		{
		case SceneManager::LoadType::NoLoad: // ロードしていない
			th_.swap(std::thread([&] { AsyncLoad(); }));
			SetLockFlag(false);
			// ロード状態=ロード始まった
			loadType_ = LoadType::LoadStart;
			break;
		case SceneManager::LoadType::LoadStart: // ロード開始
			break;
		case SceneManager::LoadType::LoadEnd: // ロード終了
			th_.join();
			// ロードの終了
			nextScene_ = nullptr;
			// 画面=ロード画面
			loadType_ = LoadType::NoLoad;
			break;
		default:
			break;
		}
	}
	
	if (GetLockFlag())
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
	if (GetLockFlag())
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
	if (GetLockFlag())
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

void SceneManager::SetLockFlag(bool isLoaded)
{
	std::lock_guard<std::mutex>  lock(isLoadedMutex);
	isLoaded_ = isLoaded;
}

bool SceneManager::GetLockFlag()
{
	std::lock_guard<std::mutex>  lock(isLoadedMutex);
	return isLoaded_;
}

void SceneManager::AsyncLoad()
{
	std::thread t = std::thread([&] { Initialize(); });

	//ダミーで10秒待つ
	auto sleepTime = std::chrono::seconds(2);
	std::this_thread::sleep_for(sleepTime);

	t.join();

	SetLockFlag(true);
}
