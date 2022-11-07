#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Collision.h"
#include "Hunter.h"
#include "Object3d.h"
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
	const float MIN_ANGLE = 2.0f;
	// 最大アングル
	const float MAX_ANGLE = 10.0f;
	// 弧度法
	const float PI = 3.14159265359f;
	// 度数法
	const float ANGLE = 180.0f;

public: // 定数
	const float MAX_HP = 500.0f;

public: // サブクラス
	enum class AnimationType
	{
		Stop,
		Move,
		Assault,
		TailAttack,
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
	static std::unique_ptr<Monster> Create(Camera* camera, Hunter* hunter);

public: // メンバ関数
	// 初期化
	void Initialize(Camera* camera);
	//終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// 向きの設定
	void AngleAdjustment();
	// 当たったか
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	// ダメージを受けたか
	void DamageHit(Sphere hitSphere);
	// 基本のアニメーション
	void Animation(AnimationType type);
	// 部位破壊
	void PartsTailDestruction();
	// ビヘイビアツリー
	void BehaviorTree();
	// 咆哮
	bool Howl();
	// 攻撃
	bool AttackMode();
	// 待機
	bool WaitingMode();
	// 死亡
	bool Dead();
	// 攻撃の経過時間
	bool AttackElapsedTime();
	// 攻撃の選択
	bool AttackModeSelection();
	// 攻撃の実行
	bool AttackModeMove();
	// 突進
	bool AttackMode1();
	// 尻尾攻撃
	bool AttackMode2();
	// 待機の経過時間
	bool WaitingElapsedTime();
	// 待機の選択
	bool WaitingModeSelection();
	// 待機の実行
	bool WaitingModeMove();
	// 何もしない
	bool WaitingMode1();
	// 接近
	bool WaitingMode2();
	// 間合いを取る
	bool WaitingMode3();
	// ビヘイビアツリーのリセット
	void TreeReset();

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
#pragma region
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
	std::unique_ptr<ParticleEmitter> blood_;
	std::unique_ptr<ObjParticle> testBlood_;
#pragma endregion モデル
#pragma region
	// 尻尾の耐久値
	int tailDestruction_ = 0;
	//ヒットポイント
	float hp_ = MAX_HP;
	// 進行方向の保存
	XMFLOAT3 saveVector_ = {};
	// ダメージを受けたか
	bool damageHitFlag_ = false;
	// 当たったか
	bool hitFlag_ = false;
	// 死んだか
	bool isDead_ = false;
	// 尻尾が切れたか
	bool tailDestructionFlag_ = false;
#pragma endregion ステータス
#pragma region 
	// sin波のタイマー
	float waveTimer_ = 0.0f;
#pragma endregion アニメーション
#pragma region 
	// 行動のセレクター
	std::vector<std::function<bool()>> activitySelector_;
	// 攻撃のシークエンス
	std::vector<std::function<bool()>> attackSequence_;
	// 待機のシークエンス
	std::vector<std::function<bool()>> waitingSequence_;
	// 攻撃のセレクター
	std::vector<bool> attackSelector_;
	// 待機のセレクター
	std::vector<bool> waitingSelector_;
	// 攻撃の経過時間
	int attackElapsedTimer_ = 0;
	// 待機の経過時間
	int waitingElapsedTimer_ = 0;
	// 咆哮したか
	bool howlflag_ = false;
	// 追尾の終了
	bool trackingEnd_ = false;
	// 攻撃の終了
	bool attackEnd_ = false;
	// 待機の終了
	bool waitingEnd_ = false;
#pragma endregion ビヘイビアツリー
};