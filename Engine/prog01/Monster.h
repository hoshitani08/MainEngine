#pragma once
#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Hunter.h"
#include "FbxObject3d.h"

enum class Phase
{
	Approach,
	Stop,
	Attack,
	Leave
};

enum class AttackType
{
	Weak,
	Ordinary,
	Strong
};

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
	// 動き
	void Move();
	// プレイヤーの設定
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// 死んだかの取得
	bool GetIsDead() { return isDead_; }
	// 向きの設定
	void AngleAdjustment();
	// 攻撃が当たったか
	void Hit(float damage);
	// 攻撃の動き
	void AttackMove(float speed);
	// 接近の動き
	void ApproachMove(float speed);

	// 基本のアニメーション
	void Animation(int type);

private: // メンバ変数
	//核
	std::unique_ptr<Object3d> nucleus_;

	std::array<std::unique_ptr<Object3d>, 5> body_;
	//右腕
	std::array<std::unique_ptr<Object3d>, 3> rightArm_;
	//左腕
	std::array<std::unique_ptr<Object3d>, 3> leftArm_;
	//尻尾
	std::array<std::unique_ptr<Object3d>, 6> tail_;
	// プレイヤーのデータ
	Hunter* hunter_ = nullptr;
	// 位置の保存
	XMFLOAT3 saveVector_ = {};
	// 足すアングル
	float addAngle_ = 2.0f;
	// 行動をする時間
	int moveTimer_ = 0;
	// 行動をする時間の最大
	int max = 0;
	// 行動を決めるカウント
	int count = 0;
	// 行動の種類
	Phase phase_ = Phase::Approach;
	// 攻撃の種類
	AttackType attackType_ = AttackType::Ordinary;
	//ヒットポイント
	int hp_ = 3;
	// 当たったか
	bool hitFlag_ = false;
	// 死んだか
	bool isDead_ = false;
	// 
	int waveTimer = 0;
	// 
	int waveCount = 0;
};