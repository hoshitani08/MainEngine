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
	const XMFLOAT3& GetPosition() { return nucleus_[0]->GetPosition(); }
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
	void Hit();
	// 攻撃の動き
	void AttackMove(float speed);

private: // メンバ変数
	//核
	std::array<std::unique_ptr<Object3d>, 5> nucleus_;
	//右腕
	std::array<std::unique_ptr<Object3d>, 2> rightArm_;
	//左腕
	std::array<std::unique_ptr<Object3d>, 2> leftArm_;
	//尻尾
	std::array<std::unique_ptr<Object3d>, 3> tail_;

	Hunter* hunter_ = nullptr;

	float speed_ = 0.5f;
	bool avoidFlag_ = false;
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
	// 死んだか
	bool isDead_ = false;
};