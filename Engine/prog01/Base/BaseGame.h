#pragma once

#include <memory>

#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "PostEffect.h"

class BaseGame
{
public:
	//実行
	void Run();
	// 初期化
	virtual void Initialize();
	//終了
	virtual void Finalize();
	// 毎フレーム処理
	virtual void Update();
	// 描画
	virtual void Draw();

protected:
	bool endRequst_ = false;
	std::unique_ptr<WinApp> win_;
	std::unique_ptr<PostEffect> postEffect_;
	//シーンファクトリ
	AbstractSceneFactory* sceneFactory_ = nullptr;
};