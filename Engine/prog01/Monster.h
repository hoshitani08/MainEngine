#pragma once
#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Hunter.h"
#include "FbxObject3d.h"



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

public: // 静的メンバ関数
	static std::unique_ptr<Monster> Create();

public: // メンバ関数
	// 初期化
	void Initialize();
	//終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();

	// 座標の取得
	const XMFLOAT3& GetPosition() { return nucleus_->GetPosition(); }
	// HPの取得
	int GetHp() { return hp_; }
	// HPの設定
	void SetHp(int hp) { hp_ = hp; }
	// 行動
	void Activity();
	// プレイヤーの設定
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// 死んだかの取得
	bool GetIsDead() { return isDead_; }
	// 向きの設定
	void AngleAdjustment();
	// 攻撃が当たったか
	void DamageHit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f, float damage = 0.0f);
	// 範囲に入ったか
	void RangeHit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	//
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);

	// 移動
	void Move(float speed);
	// 基本のアニメーション
	void Animation(AnimationType type);
	// 行動の終了
	void ActEnd();

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

	// 進行方向の保存
	XMFLOAT3 saveVector_ = {};
	// プレイヤーの位置の保存
	XMFLOAT3 savePlayerPosition = {};

	// 加算アングル
	float addAngle_ = 2.0f;
	// 行動をする時間
	float moveTimer_ = 0.0f;
	// sin波のタイマー
	float waveTimer_ = 0.0f;
	//クールタイム
	float coolTimer = 0.0f;

	// 行動をする時間の最大
	int maxTime_ = 0;
	// 行動を決めるカウント
	int actCount_ = 0;
	//ヒットポイント
	int hp_ = 3;

	// 当たったか
	bool hitFlag_ = false;
	// 保存出来るか
	bool saveFlag_ = false;
	// 死んだか
	bool isDead_ = false;
	// 行動の終了
	bool actEndFlag_ = false;

	// 行動の種類
	Phase phase_ = Phase::Approach;
	// 攻撃の種類
	AttackType attackType_ = AttackType::Ordinary;
};