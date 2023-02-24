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
#include "Ease.h"
#include "Collision.h"

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
	const float ANGLE_RESTRICTION = 0.6f;

public: // サブクラス
	// アニメーションのタイプ
	enum class AnimationType
	{
		HALT,
		MOVE,
		DAMAGE,
		DEATH,
		DASH,
		DODGE,
		COMBO1,
		COMBO2,
		COMBO3,
	};

	// アニメーションフラグ
	struct AnimationFlag
	{
		bool halt = false;
		bool move = false;
		bool damage = false;
		bool attack1 = false;
		bool attack2 = false;
		bool attack3 = false;
		bool death = false;
		bool dash = false;
		bool dodge = false;
	};

	// タイムデータ
	struct TimeData
	{
		int count = 0;
		int timer = 0;
	};

public: // 静的メンバ関数
	/// <summary>
	/// 生成
	/// </summary>
	/// <returns>Hunter</returns>
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
	const XMFLOAT3& GetPosition() { return hunter_[animationType_]->GetPosition(); }
	/// <summary>
	/// X,Y,Z軸回りの取得
	/// </summary>
	/// <returns>X,Y,Z軸回りの回転</returns>
	const XMFLOAT3& GetRotation() { return hunter_[animationType_]->GetRotation(); }
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
	bool GetActFlag() { return actFlag_; }
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
	/// アイテム選択のイージングが終わったか
	/// </summary>
	/// <returns></returns>
	bool GetIsItemSelection() { return isItemSelectionEase_; }
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetButtonFlag() { return buttonFlag_; }
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetButtonEaseFlag() { return buttonEaseFlag_; }
	/// <summary>
	/// 動いているか
	/// </summary>
	/// <returns></returns>
	bool IsMoveFlag()
	{
		if (falg_.move || falg_.dash)
		{
			return true;
		}
		return false;
	}
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
	void SetAttackFlag(bool isAttackFlag);
	/// <summary>
	/// コンボ用
	/// </summary>
	/// <param name="f"></param>
	void SetActFalg(bool f) { actFlag_ = f; }
	/// <summary>
	/// アイテム選択のイージングが終わったか
	/// </summary>
	/// <param name="f"></param>
	void SetIsItemSelection(bool f) { isItemSelectionEase_ = f; }
	/// <summary>
	/// ボタンフラグの設定
	/// </summary>
	/// <param name="f"></param>
	void SetButtonFlag(bool f) { buttonFlag_ = f; }
	/// <summary>
	/// ボタン用のイージングデータフラグの設定
	/// </summary>
	/// <param name="f"></param>
	void SetButtonEaseFlag(bool f) { buttonEaseFlag_ = f; }
	/// <summary>
	/// 攻撃した時の当たり判定の取得
	/// </summary>
	/// <returns>判定</returns>
	Sphere GetAttackHit();

private: // メンバ関数(アニメーション用)
	/// <summary>
	/// 停止
	/// </summary>
	void Halt();
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// ダメージを受けた
	/// </summary>
	void Damage();
	/// <summary>
	/// 死亡
	/// </summary>
	void Death();
	/// <summary>
	/// ダッシュ
	/// </summary>
	void Dash();
	/// <summary>
	/// 回避
	/// </summary>
	void Dodge();
	/// <summary>
	/// コンボ攻撃1
	/// </summary>
	void Combo1();
	/// <summary>
	/// コンボ攻撃2
	/// </summary>
	void Combo2();
	/// <summary>
	/// コンボ攻撃3
	/// </summary>
	void Combo3();

private: // メンバ変数
	//　モデル
	std::array<std::unique_ptr<FbxObject3d>, 9> hunter_;
	std::unique_ptr<Object3d> weapon_;
	// アイテム用パーティクル
	std::unique_ptr<ParticleEmitter> itemEmitter_;
	std::unique_ptr<ObjParticle> itemParticle_;
	std::unique_ptr<ParticleEmitter> healEmitter_;
	std::unique_ptr<ObjParticle> healParticle_;

	// アニメーション関数の管理
	std::vector<std::function<void()>> animationFunc_;

	// カメラの角度
	XMFLOAT2 cameraAngle_ = {};
	XMFLOAT2 playerAngle_ = {};

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
	// アニメーション用のイージングデータ
	std::unique_ptr<EaseData> animationEaseData_;

	// 移動倍率
	XMFLOAT2 speed_ = {};
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
	// アイテム選択のイージングが終わったか
	bool isItemSelectionEase_ = false;
	// 生きているかどうか
	bool isDeath_ = false;
	// ダッシュしているか
	bool isDash_ = false;
	// 攻撃を継続するか
	bool comboFlag_ = false;
	// 攻撃が切れたか
	bool actFlag_ = false;
	// ボタン
	bool buttonFlag_ = false;
	// ボタンのイージング
	bool buttonEaseFlag_ = false;
	// 動いているか
	bool isMoveFlag_ = false;
	// アニメーションのタイプフラグ
	AnimationFlag falg_ = {};
};