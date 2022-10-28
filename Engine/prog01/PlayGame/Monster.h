#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Collision.h"
#include "Hunter.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "ObjParticle.h"

class Monster
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

private: // 定数
	// 最小アングル
	const float MIN_ANGLE = 2;
	// 最大アングル
	const float MAX_ANGLE = 10;
	// 弧度法
	const float PI = 3.14159265359f;
	// 度数法
	const float ANGLE = 180.0f;

public: // 定数
	const float MAX_HP = 500.0f;

public: // サブクラス
	enum class Phase
	{
		Approach,
		Stop,
		Attack,
		Leave,
		CoolTime
	};

	enum class AttackType
	{
		Weak,
		Ordinary,
		Strong
	};

	enum class AnimationType
	{
		Stop,
		Move,
		Assault,
	};

	enum class PartsDamage
	{
		Body = 10,
		RightForeFoot = 5,
		LeftForeFoot = 5,
		RightHindFoot = 5,
		LeftHindFoot = 5,
		Tail = 8,
	};

public: // 静的メンバ関数
	static std::unique_ptr<Monster> Create(Camera* camera);

public: // メンバ関数
	// 初期化
	void Initialize(Camera* camera);
	//終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();
	// 行動
	void Activity();
	// 向きの設定
	void AngleAdjustment();
	// 攻撃が当たったか
	void AttackHit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f, float damage = 0.0f);
	// 範囲に入ったか
	void RangeHit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	// 当たったか
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	// ダメージを受けたか
	void DamageHit(Sphere hitSphere);
	// 移動
	void Move(float speed);
	// 基本のアニメーション
	void Animation(AnimationType type);
	// 行動の終了
	void ActEnd();
	// 血のパーティクル
	void ParticleDraw();
	// 部位破壊
	void PartsTailDestruction();

public: // メンバ関数
	// 座標の取得
	const XMFLOAT3& GetPosition() { return nucleus_->GetPosition(); }
	// X,Y,Z軸回りの取得
	const XMFLOAT3& GetRotation() { return nucleus_->GetRotation(); }
	// HPの取得
	float GetHp() { return hp_; }
	// 死んだかの取得
	bool GetIsDead() { return isDead_; }

	// プレイヤーの設定
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// HPの設定
	void SetHp(float hp) { hp_ = hp; }

private: // メンバ変数
	//核
	std::unique_ptr<Object3d> nucleus_;
	// 体
	std::array<std::unique_ptr<Object3d>, 5> body_;
	//右前足
	std::array<std::unique_ptr<Object3d>, 3> rightForeFoot_;
	//左前足
	std::array<std::unique_ptr<Object3d>, 3> leftForeFoot_;
	//右後ろ足
	std::array<std::unique_ptr<Object3d>, 3> rightHindFoot_;
	//左後ろ足
	std::array<std::unique_ptr<Object3d>, 3> leftHindFoot_;
	//尻尾
	std::array<std::unique_ptr<Object3d>, 4> tail_;
	// プレイヤーのデータ
	Hunter* hunter_ = nullptr;
	// 血のパーティクル
	std::unique_ptr<ParticleManager> particleManager_;
	std::unique_ptr<ParticleEmitter> blood_;

	std::unique_ptr<ObjParticle> testBlood_;

	// 進行方向の保存
	XMFLOAT3 saveVector_ = {};

	// 行動をする時間
	float moveTimer_ = 0.0f;
	// sin波のタイマー
	float waveTimer_ = 0.0f;
	//クールタイム
	float coolTimer = 0.0f;
	//ヒットポイント
	float hp_ = MAX_HP;

	// 行動をする時間の最大
	int maxTime_ = 0;
	// 行動を決めるカウント
	int actCount_ = 0;
	//
	int tailDestruction_ = 0;

	// ダメージを受けたか
	bool damageHitFlag_ = false;
	// 当たったか
	bool hitFlag_ = false;
	// 保存出来るか
	bool saveFlag_ = false;
	// 死んだか
	bool isDead_ = false;
	// 行動の終了
	bool actEndFlag_ = false;
	// 尻尾が切れたか
	bool tailDestructionFlag_ = false;

	// 行動の種類
	Phase phase_ = Phase::Approach;
	// 攻撃の種類
	AttackType attackType_ = AttackType::Ordinary;
};