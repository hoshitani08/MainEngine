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
#include "BehaviorTree.h"

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
	// アニメーションのタイプ
	enum class AnimationType
	{
		InitialPosture,
		Move,
		Assault,
		TailAttack,
		RightPunch,
		LeftPunch,
		Waiting,
		Tornado,
	};

	// パーツごとに受けるダメージの設定
	enum class PartsDamage
	{
		Body = 10,
		RightForeFoot = 5,
		LeftForeFoot = 5,
		RightHindFoot = 5,
		LeftHindFoot = 5,
		Tail = 8,
	};

	// パーツ
	enum class Parts
	{
		Body,
		RightForeFoot,
		LeftForeFoot,
		RightHindFoot,
		LeftHindFoot,
		Tail,
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
	/// <returns>当たったかどうか</returns>
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	/// <summary>
	/// ダメージ判定
	/// </summary>
	/// <param name="hitSphere">球</param>
	void DamageHit(Sphere hitSphere);
	/// <summary>
	/// 部位破壊
	/// </summary>
	void PartsTailDestruction();
	/// <summary>
	/// 咆哮
	/// </summary>
	/// <returns></returns>
	bool Howl();
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
	/// 右パンチ攻撃
	/// </summary>
	/// <returns></returns>
	bool AttackMode3();
	/// <summary>
	/// 左パンチ攻撃
	/// </summary>
	/// <returns></returns>
	bool AttackMode4();
	/// <summary>
	/// 範囲攻撃
	/// </summary>
	/// <returns></returns>
	bool AttackMode5();
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
	/// <summary>
	/// ダメージ計算
	/// </summary>
	/// <param name="partsDamage">ダメージ量</param>
	/// <param name="parts">どこの部位か</param>
	/// <param name="hitSphere">判定</param>
	/// <param name="temp">部位のオブジェクト</param>
	void DamageCalculate(float partsDamage, Parts parts, Sphere hitSphere, Object3d* temp);
	/// <summary>
	/// 切れた尻尾による攻撃
	/// </summary>
	void TailBullet();

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

private: // メンバ関数(アニメーション用)
	/// <summary>
	/// 初期姿勢
	/// </summary>
	void InitialPosture();
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// 突進
	/// </summary>
	void Assault();
	/// <summary>
	/// 尻尾攻撃
	/// </summary>
	void TailAttack();
	/// <summary>
	/// 右パンチ
	/// </summary>
	void RightPunch();
	/// <summary>
	/// 左パンチ
	/// </summary>
	void LeftPunch();
	/// <summary>
	/// 待機
	/// </summary>
	void Waiting();
	/// <summary>
	/// トルネード
	/// </summary>
	void Tornado();

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
	// 血のエミッター
	std::unique_ptr<ParticleEmitter> bloodEmitter_;
	// 血のパーティクル
	std::unique_ptr<ObjParticle> bloodParticle_;
	// 泡のエミッター
	std::unique_ptr<ParticleEmitter> bubbleEmitter_;
	// 泡のパーティクル
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
	// カラーを変えるためのフラグ
	bool colorChangeFlag_ = false;
	// 尻尾が動くまでのタイマー
	int tailMoveTimer_ = 0;
	// 範囲攻撃の範囲
	float attackRange_ = 1.0f;
	//
	int tornadoTimer_ = 0;
	// 尻尾の移動の方向のリセットフラグ
	std::array<bool, 4> tailMoveResetFlag_;
	std::array<bool, 4> tailMoveChangeFlag_;
	// 尻尾が動く移動量の保存
	XMFLOAT3 saveTailMoveVector_ = {};
#pragma endregion ステータス
#pragma region 
	// アニメーション関数の管理
	std::vector<std::function<void()>> animationFunc_;
	// sin波のタイマー
	float waveTimer_ = 0.0f;
	// イージングの進行度用
	float easeTimer_ = 0.0f;
	float angleEaseTimer_ = 0.0f;
	// イージングし終わったか
	bool isEaseFlag_ = false;
	// 足すアングル
	float addAngle_ = 5.0f;
	// パンチし終わったか
	bool isPunch_ = false;
	// 行動の経過したカウント
	int count_ = 0;
	// 行動の経過したカウント最大
	int maxCount_ = 0;
	// 色変化のタイマー
	int colorTimer_ = 0;
	// 咆哮する時間
	int howlTimer_ = 0;
#pragma endregion アニメーション
#pragma region 
	// ビヘイビアツリー
	std::unique_ptr<BehaviorTree> behaviorTree_;
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
};