#pragma once

#include <memory>
#include <future>

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

class SceneManager final
{
public: // サブクラス
	enum class LoadType
	{
		NoLoad,
		LoadStart,
		LoadEnd
	};

private: // シングルトン
	SceneManager() = default;
	~SceneManager();

public: // シングルトン
	SceneManager(const SceneManager& sceneManager) = delete;
	SceneManager& operator=(const SceneManager& sceneManager) = delete;
	static SceneManager* GetInstance();

public:
	// 終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();
	// エフェクト描画
	void EffectDraw();
	// 次シーンの予約
	void ChangeScene(const std::string& sceneName);
	// シーン生成の設定
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	// ロードシーンの設定
	void CreateLoadScene(const std::string& sceneName);
	// ロードフラグの設定
	void SetLockFlag(bool isLoaded);
	// ロードフラグの取得
	bool GetLockFlag();
	//  非同期ロード
	void AsyncLoad();

private:
	//今のシーン
	std::unique_ptr<BaseScene> scene_;
	//次のシーン
	std::unique_ptr<BaseScene> nextScene_;
	//ロードシーン
	std::unique_ptr<BaseScene> loadScene_;
	//シーンファクトリ
	AbstractSceneFactory* sceneFactory_ = nullptr;
	// 非同期処理
	std::thread th_ = {};
	// ロード状態
	LoadType loadType_ = LoadType::NoLoad;
	// ロードしているか
	bool isLoaded_ = false;
	// スレッド間で使用する共有リソースを排他制御する
	std::mutex isLoadedMutex = {};
};