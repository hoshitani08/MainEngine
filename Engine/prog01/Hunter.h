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

public: // サブクラス

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
	// アイテムの仕様
	void ItemUse();
	// ダメージの計算
	void DamageHit();

public: // メンバ関数
	// 座標の取得
	const XMFLOAT3& GetPosition() { return hunter_->GetPosition(); }
	// X,Y,Z軸回りの取得
	const XMFLOAT3& GetRotation() { return hunter_->GetRotation(); }
	// 無敵時間を取得
	int GetInvincibleTimer() { return invincibleTimer_; }
	// アイテムタイプの取得
	int GetItemType() { return itemType_; }
	// HPの取得
	float GetHp() { return hp_; }
	// スタミナの取得
	float GetStamina() { return stamina_; }
	// 攻撃をしたか
	bool IsAttackFlag() { return isAttackFlag_; }
	// 回避フラグ
	bool GetAvoidFlag() { return avoidFlag_; }
	// ダメージのフラグを取得
	bool GetDamageFlag() { return damageFlag_; }

	// アングルの設定
	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }
	// ダメージのパーセントを設定
	void SetDamage(float damage) { damage_ = damage; }
	// HPの設定
	void SetHp(float hp) { hp_ = hp; }
	// スタミナの設定
	void SetStamina(float stamina) { stamina_ = stamina; }
	// ダメージのフラグを設定
	void SetDamageFlag(bool damageFlag) { damageFlag_ = damageFlag; }
	// 攻撃フラグの設定
	void AttackHit(bool isAttackFlag);

private: // メンバ変数
	//　モデル
	std::unique_ptr<FbxObject3d> hunter_;

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
};