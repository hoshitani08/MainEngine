#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "CollisionPrimitive.h"
#include "Camera.h"
#include "LightGroup.h"
#include "FbxObject3d.h"
#include "Hunter.h"
#include "Monster.h"
#include "UserInterface.h"
#include "Block.h"

class CollisionManager;
class Player;
class ContactableObject;

class GameScene : public BaseScene
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

public: // サブクラス
	struct Quest
	{
		int timer = 0;
		int second = 0;
		int minute = 0;
	};

private: // 静的定数
	static const int DEBUG_TEXT_TEX_NUMBER = 0;

private: // 定数
	const XMFLOAT2 RESTRICTION_ANGLE = { 360.0f, 80.0f };

public: // メンバ関数
	// デストラクタ
	~GameScene();
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
	// カメラの動き
	void CameraMove();
	// プレイヤーの攻撃判定
	void PlayerAttack();

private: // メンバ変数
	//カメラ
	std::unique_ptr<Camera> camera_;
	//スプライト
	std::unique_ptr<Sprite> sprite_;
	//UI
	std::unique_ptr<UserInterface> ui_;
	//パーティクル
	std::unique_ptr<ParticleManager> particleMan_;
	//オブジェクト
	std::unique_ptr<Object3d> skydome_;
	std::unique_ptr<Object3d> ground_;
	std::unique_ptr<Hunter> hunter_;
	std::unique_ptr<Monster> monster_;
	std::unique_ptr<Object3d> hitSphere_;

	std::unique_ptr<Block> block_;

	//ライト
	std::unique_ptr<LightGroup> light_;
	//光線方向初期値
	float circleShadowDir_[3] = { 0,-1,0 };
	float circleShadowPos_[3] = { 1,2,0 };
	float circleShadowAtten_[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle_[2] = { 0.0f, 0.5f };

	float fighterPos[3] = { 1, 0.0f, 0 };
	//当たり判定
	CollisionManager* collisionManager_ = nullptr;
	// 角度
	XMFLOAT2 angle_ = { 0.0f, 0.0f};

	// タイマー
	Quest quest_;
};