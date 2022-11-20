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
	// デストラクタ
	~TitleScene();
	// 初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	// 毎フレーム処理
	void Update() override;
	// 描画
	void Draw() override;
	// エフェクト描画
	void EffectDraw() override;

	// ボタン関係
	void Select();
	void Shake();
	// イージング
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
	std::unique_ptr<Object3d> ground_;
	std::vector<std::unique_ptr<Block>> block_;
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