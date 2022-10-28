#pragma once

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

class SceneManager final
{
private:
	SceneManager() = default;
	~SceneManager();

public:
	SceneManager(const SceneManager& sceneManager) = delete;
	SceneManager& operator=(const SceneManager& sceneManager) = delete;

	static SceneManager* GetInstance();

	//終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();
	// エフェクト描画
	void EffectDraw();
	//次シーンの予約
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;
	//シーンファクトリ
	AbstractSceneFactory* sceneFactory_ = nullptr;
};