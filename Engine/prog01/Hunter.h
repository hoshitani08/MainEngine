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

	void Move();

	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }

	// 座標の取得
	const XMFLOAT3& GetPosition() { return position_; }
	// X,Y,Z軸回りの取得
	const XMFLOAT3& GetRotation() { return hunter_->GetRotation(); }
	// ダメージのパーセントを取得
	float GetDamagePercent() { return damagePercent_; }
	// ダメージのフラグを取得
	bool GetDamageFlag() { return damageFlag_; }
	// 回避フラグ
	bool GetAvoidFlag() { return avoidFlag_; }
	// 無敵時間を取得
	int GetInvincibleTimer() { return invincibleTimer_; }
	//スタミナの減少値を取得
	float GetStrengthDecrement() { return strengthDecrement_; }
	// ダメージのフラグを設定
	void SetDamageFlag(bool damageFlag);
	// ダメージのパーセントを設定
	void SetDamagePercent(float damagePercent) { damagePercent_ = damagePercent; }
	// 攻撃をしたか
	bool IsAttackFlag() { return isAttackFlag_; }

private: // メンバ変数
	//　モデル
	std::unique_ptr<FbxObject3d> hunter_;
	// 位置
	XMFLOAT3 position_;
	// 移動倍率
	float speed_ = 0.0f;
	// 回避フラグ
	bool avoidFlag_ = false;
	// 回避タイマー
	int avoidTimer_ = 0;
	// カメラの角度
	XMFLOAT2 cameraAngle_ = {};
	// ダメージ
	float damagePercent_ = 0.0f;
	// ダメージフラグ
	bool damageFlag_ = false;
	// 無敵時間
	int invincibleTimer_ = 300;
	// スタミナの減少値
	float strengthDecrement_ = 0.0f;
	// 攻撃が当たったか
	bool isAttackFlag_ = false;
	// 攻撃のクールタイム
	int attackCoolTimer = 0;
};