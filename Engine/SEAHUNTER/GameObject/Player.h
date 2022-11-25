#pragma once

#include "Object3d.h"
#include "ParticleManager.h"
#include "Input.h"

class Player : public Object3d
{
public:
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>インスタンス</returns>
	static Player* Create(Model* model = nullptr);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
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