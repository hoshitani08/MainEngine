#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "FbxObject3d.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "ObjParticle.h"

class Hunter
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

public: // 定数
	const float MAX_HP = 150.0f;
	const float MAX_STAMINA = 150.0f;

public: // サブクラス
	struct AnimationFlag
	{
		bool halt = false;
		bool move = false;
		bool damage = false;
		bool attack = false;
		bool death = false;
		bool dash = false;
		bool dodge = false;
	};

	struct TimeData
	{
		int count = 0;
		int timer = 0;
	};

public: // 静的メンバ関数
	static std::unique_ptr<Hunter> Create();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
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
	/// <param name="cmdList">描画コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 行動
	/// </summary>
	void Behavior();
	/// <summary>
	/// 移動
	/// </summary>
	void BaseMove();
	/// <summary>
	/// 回避
	/// </summary>
	void AvoidMove();
	/// <summary>
	/// 攻撃
	/// </summary>
	void AttackMove();
	/// <summary>
	/// スピード計算
	/// </summary>
	void SpeedCalculate();
	/// <summary>
	/// アイテムの仕様
	/// </summary>
	void ItemUse();
	/// <summary>
	/// ダメージの計算
	/// </summary>
	void DamageHit();

public: // メンバ関数
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return hunter_[0]->GetPosition(); }
	/// <summary>
	/// X,Y,Z軸回りの取得
	/// </summary>
	/// <returns>X,Y,Z軸回りの回転</returns>
	const XMFLOAT3& GetRotation() { return hunter_[0]->GetRotation(); }
	/// <summary>
	/// 無敵時間を取得
	/// </summary>
	/// <returns>無敵時間</returns>
	int GetInvincibleTimer() { return invincibleTimer_; }
	/// <summary>
	/// アイテムタイプの取得
	/// </summary>
	/// <returns>アイテムタイプ</returns>
	int GetItemType() { return itemType_; }
	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns>HP</returns>
	float GetHp() { return hp_; }
	/// <summary>
	/// スタミナの取得
	/// </summary>
	/// <returns>スタミナ</returns>
	float GetStamina() { return stamina_; }
	/// <summary>
	/// 攻撃フラグの取得
	/// </summary>
	/// <returns>攻撃フラグ</returns>
	bool IsAttackFlag() { return isAttackFlag_; }
	/// <summary>
	/// 回避フラグの取得
	/// </summary>
	/// <returns>回避フラグ</returns>
	bool GetAvoidFlag() { return avoidFlag_; }
	/// <summary>
	/// ダメージフラグを取得
	/// </summary>
	/// <returns>ダメージフラグ</returns>
	bool GetDamageFlag() { return damageFlag_; }
	/// <summary>
	/// 攻撃アニメーションが動いているか
	/// </summary>
	/// <returns>攻撃フラグ</returns>
	bool GetAnimationType() { return falg_.attack; }
	/// <summary>
	/// アイテムを選択しているか
	/// </summary>
	/// <returns>選択フラグ</returns>
	bool GetItemSelectionFlag() { return itemSelectionFlag_; }
	/// <summary>
	/// 生存フラグの取得
	/// </summary>
	/// <returns>生存フラグ</returns>
	bool GetIsDeath() { return isDeath_; }
	/// <summary>
	/// アングルの設定
	/// </summary>
	/// <param name="angle">アングル</param>
	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }
	/// <summary>
	/// ダメージのパーセントを設定
	/// </summary>
	/// <param name="damage">ダメージ</param>
	void SetDamage(float damage) { damage_ = damage; }
	/// <summary>
	/// HPの設定
	/// </summary>
	/// <param name="hp">HP</param>
	void SetHp(float hp) { hp_ = hp; }
	/// <summary>
	/// スタミナの設定
	/// </summary>
	/// <param name="stamina">スタミナ</param>
	void SetStamina(float stamina) { stamina_ = stamina; }
	/// <summary>
	/// ダメージフラグを設定
	/// </summary>
	/// <param name="damageFlag">ダメージフラグ</param>
	void SetDamageFlag(bool damageFlag) { damageFlag_ = damageFlag; }
	/// <summary>
	/// 攻撃フラグの設定
	/// </summary>
	/// <param name="isAttackFlag">攻撃フラグ</param>
	void AttackHit(bool isAttackFlag);

private: // メンバ変数
	//　モデル
	std::array<std::unique_ptr<FbxObject3d>, 7> hunter_;
	std::unique_ptr<Object3d> buki_;
	// アイテム用パーティクル
	std::unique_ptr<ParticleEmitter> itemEmitter_;
	std::unique_ptr<ObjParticle> itemParticle_;
	std::unique_ptr<ParticleEmitter> healEmitter_;
	std::unique_ptr<ObjParticle> healParticle_;

	// カメラの角度
	XMFLOAT2 cameraAngle_ = {};

	// 回避タイマー
	int avoidTimer_ = 0;
	// 無敵時間
	int invincibleTimer_ = 300;
	// 攻撃のクールタイム
	int attackCoolTimer_ = 0;
	// アイテムのタイプ
	int itemType_ = 0;
	// アニメーションのタイプ
	int animationType_ = 0;
	// 攻撃の効果の時間
	TimeData attack;
	// 攻撃の効果の時間
	TimeData defense;

	// 移動倍率
	float speed_ = 0.0f;
	// ヒットポイント
	float hp_ = MAX_HP;
	// スタミナ
	float stamina_ = MAX_STAMINA;
	// ダメージ量
	float damage_ = 0.0f;

	// 回避フラグ
	bool avoidFlag_ = false;
	// ダメージフラグ
	bool damageFlag_ = false;
	// スタミナが残ってるか
	bool isStamina_ = true;
	// 攻撃が当たったか
	bool isAttackFlag_ = false;
	// アイテムの選択してるか
	bool itemSelectionFlag_ = false;
	// 生きているかどうか
	bool isDeath_ = false;
	//
	bool isDash = false;
	// アニメーションのタイプフラグ
	AnimationFlag falg_ = {};
};