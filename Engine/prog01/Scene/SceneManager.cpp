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
	scene_ = nullptr;

	if (nextScene_ != nullptr)
	{
		th_.detach();
		nextScene_->Finalize();
		nextScene_ = nullptr;
	}

	if (loadScene_ != nullptr)
	{
		th_.detach();
		loadScene_->Finalize();
		loadScene_ = nullptr;
	}
}

void SceneManager::Update()
{
	if (nextScene_)
	{
		switch (loadType_)
		{
		case SceneManager::LoadType::NoLoad: // ロードしていない
			th_.swap(std::thread([&] { AsyncLoad(); }));
			// 
			scene_ = std::move(loadScene_);
			// ロード状態=ロード始まった
			loadType_ = LoadType::LoadStart;
			break;
		case SceneManager::LoadType::LoadStart: // ロード開始
			break;
		case SceneManager::LoadType::LoadEnd: // ロード終了
			th_.join();
			// ロードの終了
			scene_ = std::move(nextScene_);
			// 画面=ロード画面
			loadType_ = LoadType::NoLoad;
			break;
		default:
			break;
		}
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

	nextScene_ = std::unique_ptr<BaseScene>(sceneFactory_->CreateScene(sceneName));
	CreateLoadScene("LoadScene");
}

void SceneManager::CreateLoadScene(const std::string& sceneName)
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
	std::thread t = std::thread([&] { nextScene_->Initialize(); });

	//ダミーで10秒待つ
	auto sleepTime = std::chrono::seconds(2);
	std::this_thread::sleep_for(sleepTime);

	t.join();

	loadType_ = LoadType::LoadEnd;
}
