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
	/// <summary>
	/// 実行
	/// </summary>
	void Run();
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

protected:
	bool endRequst_ = false;
	std::unique_ptr<WinApp> win_;
	std::unique_ptr<PostEffect> postEffect_;
	//シーンファクトリ
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
};