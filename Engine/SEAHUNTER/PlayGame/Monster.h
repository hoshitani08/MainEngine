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
#include "Vector.h"

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
	// 弧度法
	const float PI = 3.14159265359f;
	// 度数法
	const float ANGLE = 180.0f;

public: // 定数
	const float MAX_HP = 500.0f;

public: // サブクラス
	enum class AnimationType
	{
		InitialPosture,
		Move,
		Assault,
		TailAttack,
		Punch,
		Waiting,
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
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="hunter">プレイヤー</param>
	/// <returns>生成されたオブジェクト</returns>
	static std::unique_ptr<Monster> Create(Camera* camera, Hunter* hunter);

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Initialize(Camera* camera);
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
	/// 全ての動き
	/// </summary>
	void AllMove();
	/// <summary>
	/// 向きの設定
	/// </summary>
	void AngleAdjustment();
	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="partsPosition">座標</param>
	/// <param name="enemyRange">半径</param>
	/// <param name="playerRange">半径</param>
	/// <returns></returns>
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	/// <summary>
	/// ダメージ判定
	/// </summary>
	/// <param name="hitSphere">球</param>
	void DamageHit(Sphere hitSphere);
	/// <summary>
	/// 基本のアニメーション
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	void Animation(AnimationType type);
	/// <summary>
	/// 部位破壊
	/// </summary>
	void PartsTailDestruction();
	/// <summary>
	/// ビヘイビアツリー
	/// </summary>
	void BehaviorTree();
	/// <summary>
	/// 咆哮
	/// </summary>
	/// <returns></returns>
	bool Howl();
	/// <summary>
	/// 攻撃
	/// </summary>
	/// <returns></returns>
	bool AttackMode();
	/// <summary>
	/// 待機
	/// </summary>
	/// <returns></returns>
	bool WaitingMode();
	/// <summary>
	/// 死亡
	/// </summary>
	/// <returns></returns>
	bool Dead();
	/// <summary>
	/// 攻撃の経過時間
	/// </summary>
	/// <returns></returns>
	bool AttackElapsedTime();
	/// <summary>
	/// 攻撃の選択
	/// </summary>
	/// <returns></returns>
	bool AttackModeSelection();
	/// <summary>
	/// 攻撃の実行
	/// </summary>
	/// <returns></returns>
	bool AttackModeMove();
	/// <summary>
	/// 突進
	/// </summary>
	/// <returns></returns>
	bool AttackMode1();
	/// <summary>
	/// 尻尾攻撃
	/// </summary>
	/// <returns></returns>
	bool AttackMode2();
	/// <summary>
	/// パンチ攻撃
	/// </summary>
	/// <returns></returns>
	bool AttackMode3();
	/// <summary>
	/// 待機の経過時間
	/// </summary>
	/// <returns></returns>
	bool WaitingElapsedTime();
	/// <summary>
	/// 待機の選択
	/// </summary>
	/// <returns></returns>
	bool WaitingModeSelection();
	/// <summary>
	/// 待機の実行
	/// </summary>
	/// <returns></returns>
	bool WaitingModeMove();
	/// <summary>
	/// 何もしない
	/// </summary>
	/// <returns></returns>
	bool WaitingMode1();
	/// <summary>
	/// 接近
	/// </summary>
	/// <returns></returns>
	bool WaitingMode2();
	/// <summary>
	/// 間合いを取る
	/// </summary>
	/// <returns></returns>
	bool WaitingMode3();
	/// <summary>
	/// ビヘイビアツリーのリセット
	/// </summary>
	void TreeReset();

public: // メンバ関数
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return nucleus_->GetPosition(); }
	/// <summary>
	/// X,Y,Z軸回りの取得
	/// </summary>
	/// <returns>X,Y,Z軸回りの回転</returns>
	const XMFLOAT3& GetRotation() { return nucleus_->GetRotation(); }
	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns>HP</returns>
	float GetHp() { return hp_; }
	/// <summary>
	/// 死亡フラグの取得
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool GetIsDead() { return isDead_; }
	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="hunter">プレイヤー</param>
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	/// <summary>
	/// HPの設定
	/// </summary>
	/// <param name="hp">HP</param>
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
	std::unique_ptr<ParticleEmitter> bloodEmitter_;
	std::unique_ptr<ObjParticle> bloodParticle_;
	// 泡のパーティクル
	std::unique_ptr<ParticleEmitter> bubbleEmitter_;
	std::unique_ptr<ObjParticle> bubbleParticle_;
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
	// イージング用位置
	XMFLOAT3 endRotation_ = {0,360,0};
	// イージングの進行度用
	float easeTimer_ = 0.0f;
	float angleEaseTimer_ = 0.0f;
	// イージングし終わったか
	bool isEaseFlag_ = false;
	// 足すアングル
	float addAngle = 5.0f;
	// パンチし終わったか
	bool isPunch = false;
	//
	int count = 0;
	int maxCount = 0;
#pragma endregion アニメーション
#pragma region 
	// 行動のセレクター
	std::vector<std::function<bool()>> activitySelector_;
	// 攻撃のシークエンス
	std::vector<std::function<bool()>> attackSequence_;
	// 待機のシークエンス
	std::vector<std::function<bool()>> waitingSequence_;
	// 攻撃のセレクター
	std::vector<std::function<bool()>> attackSelector_;
	std::vector<bool> attackSelect_;
	// 待機のセレクター
	std::vector<std::function<bool()>> waitingSelector_;
	std::vector<bool> waitingSelect_;
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
#pragma region 
	//
	bool debugFlag = false;
#pragma endregion デバック
};