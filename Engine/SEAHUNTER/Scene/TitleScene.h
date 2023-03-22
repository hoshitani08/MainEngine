#pragma once
#include <memory>
#include <array>
#include <algorithm>

#include "BaseScene.h"
#include "Input.h"
#include "Audio.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "MapChip.h"
#include "Block.h"
#include "SceneChange.h"
#include "Stage.h"

class TitleScene : public BaseScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// エフェクトあり描画
	/// </summary>
	void EffectDraw() override;
	/// <summary>
	/// ボタン関係
	/// </summary>
	void Select();
	/// <summary>
	/// シェイク
	/// </summary>
	void Shake();
	/// <summary>
	/// イージング
	/// </summary>
	void EaseMove();

private: // メンバ変数
	//カメラ
	std::unique_ptr<Camera> camera_;
	//ライト
	std::unique_ptr<LightGroup> light_;
	//オブジェクト
	std::unique_ptr<Object3d> titleTile_;
	std::unique_ptr<Object3d> startTile_;
	std::unique_ptr<Object3d> quitTile_;
	std::unique_ptr<Stage> stage_;
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;
	// 決定フラグ
	bool determinationFlag_ = true;
	// シェイクしているか
	bool isShake_ = false;
	// シェイクタイマー
	int shakeTimer_ = 0;
	// 減衰値
	int attenuation_ = 0;
	// シェイク前の位置
	XMFLOAT3 savePos_ = {};
	// イージング用位置
	std::array<XMFLOAT3, 3> startPosition_ = {};
	std::array<XMFLOAT3, 3> endPosition_ = {};
	// イージングの進行度用
	float easeTimer_ = 0.0f;
	// イージングし終わったか
	bool isEaseFlag_ = false;
};