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
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SceneManager* GetInstance();

public:
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// エフェクトあり描画
	/// </summary>
	void EffectDraw();
	/// <summary>
	/// 次シーンの予約
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);
	/// <summary>
	/// シーン生成の設定
	/// </summary>
	/// <param name="sceneFactory">シーン生成</param>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	/// <summary>
	/// ロードシーンの設定
	/// </summary>
	void CreateLoadScene();
	/// <summary>
	/// ロードフラグの設定
	/// </summary>
	/// <param name="isLoaded"></param>
	void SetLockFlag(bool isLoaded);
	/// <summary>
	/// ロードフラグの取得
	/// </summary>
	/// <returns></returns>
	bool GetLockFlag();
	/// <summary>
	/// 非同期ロード
	/// </summary>
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