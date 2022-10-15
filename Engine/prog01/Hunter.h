#pragma once
#include <DirectXMath.h>
#include <memory>

#include "FbxObject3d.h"
#include "Object3d.h"

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

public: // 静的メンバ関数
	static std::unique_ptr<Hunter> Create();

public: // メンバ関数
	// 初期化
	void Initialize();
	//終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();
	// 行動
	void Behavior();
	// 移動
	void BaseMove();
	// 回避
	void AvoidMove();
	// 攻撃
	void AttackMove();
	// スピード計算
	void SpeedCalculate();
	// アングルの設定
	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }
	// 座標の取得
	const XMFLOAT3& GetPosition() { return hunter_->GetPosition(); }
	// X,Y,Z軸回りの取得
	const XMFLOAT3& GetRotation() { return hunter_->GetRotation(); }
	// ダメージのフラグを取得
	bool GetDamageFlag() { return damageFlag_; }
	// ダメージのフラグを設定
	void SetDamageFlag(bool damageFlag) { damageFlag_ = damageFlag; }
	// 回避フラグ
	bool GetAvoidFlag() { return avoidFlag_; }
	// 無敵時間を取得
	int GetInvincibleTimer() { return invincibleTimer_; }
	// 攻撃をしたか
	bool IsAttackFlag() { return isAttackFlag_; }
	// 攻撃フラグの設定
	void AttackHit(bool isAttackFlag);
	// ダッシュのフラグを設定
	///void SetIsDash(bool isDash) { isDash_ = isDash; }
	// ダメージのパーセントを設定
	void SetDamage(float damage) { damage_ = damage; }
	// HPの取得
	float GetHp() { return hp_; }
	// HPの設定
	void SetHp(float hp) { hp_ = hp; }
	// HPの取得
	float GetStamina() { return stamina_; }
	// HPの設定
	void SetStamina(float stamina) { stamina_ = stamina; }
	//
	void DamageHit();

private: // メンバ変数
	//　モデル
	std::unique_ptr<FbxObject3d> hunter_;
	// 移動倍率
	float speed_ = 0.0f;
	// 回避フラグ
	bool avoidFlag_ = false;
	// 回避タイマー
	int avoidTimer_ = 0;
	// カメラの角度
	XMFLOAT2 cameraAngle_ = {};
	// ダメージフラグ
	bool damageFlag_ = false;
	// 無敵時間
	int invincibleTimer_ = 300;
	// スタミナが残ってるか
	bool isStamina_ = true;
	// 攻撃が当たったか
	bool isAttackFlag_ = false;
	// 攻撃のクールタイム
	int attackCoolTimer_ = 0;
	// ヒットポイント
	float hp_ = MAX_HP;
	// スタミナ
	float stamina_ = MAX_STAMINA;
	// ダメージ量
	float damage_ = 0.0f;
};