#pragma once

#include "Object3d.h"
#include "ParticleManager.h"
#include "Input.h"

class Player : public Object3d
{
public:
	// 3Dオブジェクト生成
	static Player* Create(Model* model = nullptr);

public:
	// 初期化
	bool Initialize() override;
	// 毎フレーム処理
	void Update() override;
	// 衝突時コールバック関数
	void OnCollision(const CollisionInfo& info) override;
	// マネージャーの設定
	void SetParticleManager(ParticleManager* particleMan) { particleMan_ = particleMan; }
	void SetInput(Input* input) { input_ = input; }

private: // メンバ変数
	ParticleManager* particleMan_ = nullptr;
	Input* input_ = nullptr;
	//接地フラグ
	bool onGround_ = true;
	// 落下ベクトル
	DirectX::XMVECTOR fallV_;
};